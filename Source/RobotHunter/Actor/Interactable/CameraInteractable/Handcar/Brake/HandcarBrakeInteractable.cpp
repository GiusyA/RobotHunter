#include "HandcarBrakeInteractable.h"
#include "EnhancedInputComponent.h"
#include "RobotHunter/Player/CustomPlayer.h"
#include "RobotHunter/Actor/Handcar/HandcarActor.h"

AHandcarBrakeInteractable::AHandcarBrakeInteractable()
{
#pragma region Debug
	usePositionOnAngleScopeDebug = false;
	useDecelerationValueDebug = false;
#pragma endregion


#pragma region Position
	leverBase = CREATE_DEFAULT_SUBOBJECT(USceneComponent, "LeverBase");
	SETUP_ATTACHMENT(leverBase, RootComponent);

	leverMesh = CREATE_DEFAULT_SUBOBJECT(UStaticMeshComponent, "LeverMesh");
	SETUP_ATTACHMENT(leverMesh, leverBase);


#pragma region Angle
	tightenedAngle = 45.0f;
	releasedAngle = 135.0f;
#pragma endregion


#pragma region Movement
	tightenMinValue = 7.0f;
	tightenMaxValue = 10.0f;

	releaseSpeed = 20.0f;
#pragma endregion


	isReleased = false;
	positionOnAngleScope = 0.0f;
#pragma endregion


	handcar = nullptr;

	decelerationValue = -75.0f;
	currentDecelerationValue = -75.0f;
}


void AHandcarBrakeInteractable::TightenLever(const FInputActionValue& _value)
{
	if (leverBase && isReleased)
	{
		const FRotator _currentRot = leverBase->GetRelativeRotation();
		//The closer the lever is to be completely tightened, the harder it is to tighten it.
		const float _tightenValue = FMath::Lerp(tightenMinValue, tightenMaxValue, positionOnAngleScope);
		float _newPitch = _currentRot.Pitch + _tightenValue;

		if (_newPitch >= tightenedAngle)
		{
			isReleased = false;
			_newPitch = tightenedAngle;
		}

		leverBase->SetRelativeRotation(FRotator(_newPitch, _currentRot.Yaw, _currentRot.Roll));

		if (!isReleased)
			UpdatePositionOnAngleScope();
	}
}

void AHandcarBrakeInteractable::ReleaseLever(const FInputActionValue& _value)
{
	isReleased = true;
}


void AHandcarBrakeInteractable::ReleasingLever(const float _deltaTime)
{
	if (leverBase && isReleased)
	{
		const FRotator _currentRot = leverBase->GetRelativeRotation();

		//I want the values for the angles given in the blueprint to work like in a trigonometric circle where 0° and 180° equal an horizontal position.
		//However, in code these values are equal to a vertical position so I have to convert them here.
		const float _releasedAngle = -(180.0f - releasedAngle);

		//The release speed becomes slower the closer the lever is to be completely released (positionOnAngleScope == 1).
		const float _releaseValue = releaseSpeed * (1.0f - positionOnAngleScope) * _deltaTime;
		float _newPitch = _currentRot.Pitch - _releaseValue;

		if (FMath::IsNearlyEqual(_currentRot.Pitch, _releasedAngle, 0.1f))
			_newPitch = _releasedAngle;

		leverBase->SetRelativeRotation(FRotator(_newPitch, _currentRot.Yaw, _currentRot.Roll));
	}
}


void AHandcarBrakeInteractable::UpdatePropertiesFromDA(UHandcarPropertiesDA* _da)
{
	if (_da)
	{
		_da->UpdateUseDebug(useRealtime, useDebug, useDebugTool);
		_da->UpdateBrakeUseDebug(usePositionOnAngleScopeDebug, useDecelerationValueDebug);

		_da->UpdateBrakeAngle(tightenedAngle, releasedAngle);
		_da->UpdateBrakeTightenValue(tightenMinValue, tightenMaxValue);

		_da->UpdateBrakeDecelerationValue(decelerationValue);
	}
}

void AHandcarBrakeInteractable::UpdatePositionOnAngleScope()
{
	if (leverBase)
	{
		const FRotator _currentRot = leverBase->GetRelativeRotation();
		const float _angleScope = releasedAngle - tightenedAngle;

		//If the current angle equals tightenedAngle, the position on scope should be 0. If it equals releasedAngle, it should be 1.
		const float _positionOnScope = FMath::Abs(_currentRot.Pitch - tightenedAngle) / _angleScope;
		positionOnAngleScope = _positionOnScope;
	}
}


void AHandcarBrakeInteractable::PrintDebug() const
{
	Super::PrintDebug();

	if (usePositionOnAngleScopeDebug)
		PRINT_SCREEN_WITH_FLOAT_TICK("[Handcar][Brake] Position on angle scope : ", positionOnAngleScope, FColor::Blue);

	if (useDecelerationValueDebug)
		PRINT_SCREEN_WITH_FLOAT_TICK("[Handcar][Brake] Deceleration value : ", currentDecelerationValue, FColor::Red);
}


void AHandcarBrakeInteractable::SetupPlayerInputs(ACustomPlayer* _player)
{
	if (_player)
	{
		UEnhancedInputComponent* _input = Cast<UEnhancedInputComponent>(_player->InputComponent);
		const UInputConfigDA* _inputConfig = _player->GetInputConfig();

		if (_input && _inputConfig)
		{
			_input->BindAction(_inputConfig->GetHandcarBrakeTightenAction(), ETriggerEvent::Started, this, &AHandcarBrakeInteractable::TightenLever);
			_input->BindAction(_inputConfig->GetHandcarBrakeReleaseAction(), ETriggerEvent::Started, this, &AHandcarBrakeInteractable::ReleaseLever);
			Super::SetupPlayerInputs(_player);
		}
	}
}


void AHandcarBrakeInteractable::FirstInteraction(ACustomPlayer* _player, USceneComponent* _playerPosition)
{
	Super::FirstInteraction(_player, _playerPosition);

	if (_player)
		_player->SetFSMBool(EPlayerBool::HandcarBrakeBool, true);

	updatePlayerMeshPitch = true;
}

void AHandcarBrakeInteractable::SecondInteraction(ACustomPlayer* _player)
{
	Super::SecondInteraction(_player);

	if (_player)
		_player->SetFSMBool(EPlayerBool::HandcarBrakeBool, false);

	updatePlayerMeshPitch = false;
}


void AHandcarBrakeInteractable::BrakeBeginPlay(UHandcarPropertiesDA* _da, AHandcarActor* _handcar)
{
	UpdatePropertiesFromDA(_da);
	handcar = _handcar;

	if (leverBase)
	{
		const FRotator _currentRot = leverBase->GetRelativeRotation();
		leverBase->SetRelativeRotation(FRotator(tightenedAngle, _currentRot.Yaw, _currentRot.Roll));
	}
}

void AHandcarBrakeInteractable::BrakeTick(const float _deltaTime)
{
	if (isRuntime)
	{
		if (updatePlayerMeshPitch && handcar)
		{
			const FRotator _handcarRot = handcar->GetActorRotation();
			UpdatePlayerMeshPitch(player, playerPosition, _handcarRot.Pitch);
		}

		if (isReleased)
		{
			UpdatePositionOnAngleScope();
			ReleasingLever(_deltaTime);
		}
	}
}


float AHandcarBrakeInteractable::GetUpdateCurrentDecelerationValue(const float _handcarSpeed)
{
	const float _decelerationValue = _handcarSpeed >= 0.0f ? decelerationValue : -decelerationValue;
	//The deceleration value becomes smaller the closer the lever is to be completely released (positionOnAngleScope equals 1).
	const float _decelerationValueFromPos = _decelerationValue * (1.0f - positionOnAngleScope);
	currentDecelerationValue = _decelerationValueFromPos;
	return currentDecelerationValue;
}
