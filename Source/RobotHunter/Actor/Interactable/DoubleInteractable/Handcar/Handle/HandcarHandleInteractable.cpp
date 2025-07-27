#include "HandcarHandleInteractable.h"
#include "EnhancedInputComponent.h"
#include "RobotHunter/Player/CustomPlayer.h"
#include "RobotHunter/Actor/Handcar/HandcarActor.h"

AHandcarHandleInteractable::AHandcarHandleInteractable()
{
#pragma region Debug
	useAccelerationDebug = false;

	useFirstHeightRapportDebug = false;
	useSecondHeightRapportDebug = false;
#pragma endregion


#pragma region Mesh
	handleMesh = CREATE_DEFAULT_SUBOBJECT(UStaticMeshComponent, "HandleMesh");
	SETUP_ATTACHMENT(handleMesh, RootComponent);


#pragma region Rotation
	maxRotation = 45.0f;
	rotationValue = 50.0f;
#pragma endregion


#pragma region Height
	currentFirstHeightRapport = 0.5f;
	currentSecondHeightRapport = 0.5f;

	latestFirstHeightRapport = 0.5f;
	latestSecondHeightRapport = 0.5f;
#pragma endregion


#pragma endregion


#pragma region Acceleration
	minForce = 5000.0f;
	maxForce = 10000.0f;

	accelerationBoost = 1.5f;

	firstPlayerAccelerates = false;
	secondPlayerAccelerates = false;

	negateAcceleration = false;

	currentAcceleration = 0.0f;
#pragma endregion


	handcar = nullptr;
}


void AHandcarHandleInteractable::UpdatePropertiesFromDA(UHandcarPropertiesDA* _da)
{
	if (_da)
	{
		_da->UpdateUseDebug(useRealtime, useDebug, useDebugTool);
		_da->UpdateHandleUseDebug(useAccelerationDebug, useFirstHeightRapportDebug, useSecondHeightRapportDebug);

		_da->UpdateHandleMeshRotation(maxRotation, rotationValue);

		_da->UpdateHandleForce(minForce, maxForce);
		_da->UpdateHandleAccelerationBoost(accelerationBoost);
	}
}


void AHandcarHandleInteractable::RotateHandle(const float _currentMaxRotation, const float _rotationValue, const float _deltaTime)
{
	if (handleMesh)
	{
		const FRotator _currentRot = handleMesh->GetRelativeRotation();
		const float _rotationChange = _rotationValue * _deltaTime;

		if (FMath::IsNearlyEqual(_currentRot.Pitch, _currentMaxRotation))
			return;

		const bool _isNegatifChange = _rotationValue < 0.0f;
		const bool _resetPitch = _currentMaxRotation == 0.0f;

		const float _minPitch = _resetPitch && _isNegatifChange ? _currentMaxRotation
									: -maxRotation;

		const float _maxPitch = _resetPitch && !_isNegatifChange ? _currentMaxRotation
									: maxRotation;

		float _newPitch = _currentRot.Pitch + _rotationChange;
		CLAMP(_newPitch, _minPitch, _maxPitch);

		handleMesh->SetRelativeRotation(FRotator(_newPitch, _currentRot.Yaw, _currentRot.Roll));
	}
}

void AHandcarHandleInteractable::UpdateHandleHeightRapports()
{
	if (handleMesh)
	{
		const FRotator _currentRot = handleMesh->GetRelativeRotation();

		const float _angleScope = maxRotation * 2.0f;

		//When the handle rotation is at the lowest value possible, the height rapport is equal to 1.
		//When it's at the highest value possible, the height rapport is equal to 0.
		const float _heightRapport = 1.0f - (FMath::Abs(_currentRot.Pitch + maxRotation) / _angleScope);

		latestFirstHeightRapport = currentFirstHeightRapport;
		latestSecondHeightRapport = currentSecondHeightRapport;

		currentFirstHeightRapport = _heightRapport;
		currentSecondHeightRapport = 1.0f - currentFirstHeightRapport;
	}
}


void AHandcarHandleInteractable::UpdateCurrentAcceleration(const float _lastHeightRapport, const float _currentHeightRapport)
{
	if (_currentHeightRapport <= 0.0f)
	{
		currentAcceleration = 0.0f;
		return;
	}

	const float _heightRapport = FMath::Abs(_lastHeightRapport - _currentHeightRapport);
	float _forceLoss = 0.5f;

	if (handcar)
	{
		const float _currentSpeed = handcar->GetCurrentSpeed();
		const float _maxSpeed = handcar->GetMaxSpeed();

		//The closer the speed is of the max speed, the less force pumping gives.
		_forceLoss = FMath::Abs(_currentSpeed) / _maxSpeed;
	}

	const float _newAcceleration = _heightRapport * FMath::Lerp(maxForce, minForce, _forceLoss);
	currentAcceleration = negateAcceleration ? -_newAcceleration
							: _newAcceleration;
}

void AHandcarHandleInteractable::AddAccelerationBoost(const float _lowestHeightRapport, const float _highestHeightRapport)
{
	if (_lowestHeightRapport >= 0.1f && _lowestHeightRapport <= 0.3f)
	{
		if (_highestHeightRapport >= 0.7f && _highestHeightRapport <= 0.9f)
		{
			currentAcceleration += negateAcceleration ? -accelerationBoost
									: accelerationBoost;
		}
	}
}


void AHandcarHandleInteractable::StartAcceleration(const FInputActionValue& _value)
{
	//isDetected and secondSideDetected aren't replicated so they can be used here to check which player starts accelerating.

	if (player && isDetected)
		firstPlayerAccelerates = true;
	else if (secondPlayer && secondSideDetected)
		secondPlayerAccelerates = true;
}

void AHandcarHandleInteractable::StopAcceleration(const FInputActionValue& _value)
{
	//isDetected and secondSideDetected aren't replicated so they can be used here to check which player stops accelerating.

	if (player && isDetected)
		firstPlayerAccelerates = false;
	else if (secondPlayer && secondSideDetected)
		secondPlayerAccelerates = false;
}


void AHandcarHandleInteractable::ChangeWay(const FInputActionValue& _value)
{
	if (handcar)
		handcar->ChangeWay();
}


void AHandcarHandleInteractable::PrintDebug() const
{
	Super::PrintDebug();

	if (useAccelerationDebug)
		PRINT_SCREEN_WITH_FLOAT_TICK("[Handcar][Handle] Acceleration : ", currentAcceleration, FColor::Green);

	if (useFirstHeightRapportDebug)
		PRINT_SCREEN_WITH_FLOAT_TICK("[Handcar][Handle] First Height Rapport : ", currentFirstHeightRapport, FColor::Blue);

	if (useSecondHeightRapportDebug)
		PRINT_SCREEN_WITH_FLOAT_TICK("[Handcar][Handle] Second Height Rapport : ", currentSecondHeightRapport, FColor::Blue);
}


void AHandcarHandleInteractable::SetupPlayerInputs(ACustomPlayer* _player)
{
	if (_player)
	{
		UEnhancedInputComponent* _input = Cast<UEnhancedInputComponent>(_player->InputComponent);
		UInputConfigDA* _inputConfig = _player->GetInputConfig();

		if (_input && _inputConfig)
		{
			_input->BindAction(_inputConfig->GetHandcarHandleAcceleration(), ETriggerEvent::Started, this, &AHandcarHandleInteractable::StartAcceleration);
			_input->BindAction(_inputConfig->GetHandcarHandleAcceleration(), ETriggerEvent::Completed, this, &AHandcarHandleInteractable::StopAcceleration);

			_input->BindAction(_inputConfig->GetHandcarHandleChangeWay(), ETriggerEvent::Started, this, &AHandcarHandleInteractable::ChangeWay);
			Super::SetupPlayerInputs(_player);
		}
	}
}


void AHandcarHandleInteractable::FirstInteraction(ACustomPlayer* _player, USceneComponent* _playerPosition)
{
	Super::FirstInteraction(_player, _playerPosition);

	if (_player)
		_player->SetFSMBool(EPlayerBool::HandcarHandleBool, true);

	updatePlayerMeshRoll = true;
}

void AHandcarHandleInteractable::SecondInteraction(ACustomPlayer* _player)
{
	Super::SecondInteraction(_player);

	if (_player)
		_player->SetFSMBool(EPlayerBool::HandcarHandleBool, false);

	updatePlayerMeshRoll = false;
}


void AHandcarHandleInteractable::HandleBeginPlay(UHandcarPropertiesDA* _da, AHandcarActor* _handcar)
{
	UpdatePropertiesFromDA(_da);
	handcar = _handcar;
}

void AHandcarHandleInteractable::HandleTick(const float _deltaTime)
{
	if (isRuntime)
	{
		if (updatePlayerMeshRoll && handcar)
		{
			const FRotator _handcarRot = handcar->GetActorRotation();
			UpdatePlayerMeshRoll(player, playerPosition, _handcarRot.Pitch);
			UpdatePlayerMeshRoll(secondPlayer, secondPlayerPosition, -_handcarRot.Pitch);
		}

		UpdateHandleHeightRapports();

		if (firstPlayerAccelerates && !secondPlayerAccelerates)
		{
			UpdateCurrentAcceleration(latestFirstHeightRapport, currentFirstHeightRapport);
			AddAccelerationBoost(currentFirstHeightRapport, currentSecondHeightRapport);
			RotateHandle(-maxRotation, -rotationValue, _deltaTime);
		}
		else if (!firstPlayerAccelerates && secondPlayerAccelerates)
		{
			UpdateCurrentAcceleration(latestSecondHeightRapport, currentSecondHeightRapport);
			AddAccelerationBoost(currentSecondHeightRapport, currentFirstHeightRapport);
			RotateHandle(maxRotation, rotationValue, _deltaTime);
		}
		else
		{
			currentAcceleration = 0.0f;

			if (handleMesh)
			{
				const FRotator _currentRot = handleMesh->GetRelativeRotation();
				const float _rotationValue = _currentRot.Pitch > 0.0f ? -rotationValue
					: rotationValue;

				RotateHandle(0.0f, _rotationValue, _deltaTime);
			}
		}
	}
}
