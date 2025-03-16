#include "HandcarActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "RobotHunter/Actor/Interactable/Rail/Lever/RailLeverActor.h"
#include "RobotHunter/Player/CustomPlayer.h"

AHandcarActor::AHandcarActor()
{
#pragma region DataAsset
	useDataAsset = true;
	dataAsset = nullptr;
#pragma endregion


#pragma region Debug
	useSpeedDebug = false;
	useFrictionDebug = false;
	useSlopeAccelerationDebug = false;
	useTotalAccelerationDebug = false;
#pragma endregion


#pragma region Movement
	currentRail = nullptr;
	nextRail = nullptr;


#pragma region Speed
	maxSpeed = 500.0f;
	rotationSpeed = 25.0f;

	currentSpeed = 0.0f;
#pragma endregion


#pragma region Acceleration


#pragma region Friction
	friction = -3.0f;
	frictionSpeedPercent = 0.1f;

	currentFriction = 0.0f;
#pragma endregion


#pragma region Slope
	slopeAccelerationCurve = nullptr;

	currentSlopeAcceleration = 0.0f;
#pragma endregion


	totalAcceleration = 0.0f;
#pragma endregion


	isBlocked = false;
	onReturn = false;

	currentDistance = 0.0f;
#pragma endregion


	nitro = nullptr;
	handle = nullptr;
}


void AHandcarActor::InitLocationRotation()
{
	if (currentRail)
	{
		const FVector _stopLoc = currentRail->GetStopLocation(true);
		currentDistance = currentRail->GetDistanceAtLocation(_stopLoc);
		SetActorLocation(_stopLoc);

		const float _distance = currentDistance + 1.0f;
		const FVector _currentTargetLoc = currentRail->GetLocationAtDistance(_distance);
		const FRotator _lookAt = UKismetMathLibrary::FindLookAtRotation(_stopLoc, _currentTargetLoc);
		SetActorRotation(_lookAt);
	}
}

void AHandcarActor::UpdatePropertiesFromDA()
{
	if (dataAsset)
	{
		dataAsset->UpdateUseDebug(useRealtime, useDebug, useDebugTool);
		dataAsset->UpdateUseHandcarDebug(useSpeedDebug, useFrictionDebug, useSlopeAccelerationDebug, useTotalAccelerationDebug);

		dataAsset->UpdateSpeed(maxSpeed, rotationSpeed);
		dataAsset->UpdateFriction(friction, frictionSpeedPercent);
	}
}


#pragma region Movement
void AHandcarActor::Move(const float _deltaTime)
{
	if (currentRail)
	{
		const float _addedDistance = currentSpeed * _deltaTime;
		float _newDistance = currentDistance + _addedDistance;

		CheckUpdateIsBlocked(_newDistance);

		if (!isBlocked)
		{
			if (CheckUpdateCurrentRail(_newDistance))
				_newDistance = currentRail->GetDistanceAtLocation(GetActorLocation()) + _addedDistance;

			const FVector _newLoc = currentRail->GetLocationAtDistance(_newDistance);
			SetActorLocation(_newLoc);
			currentDistance = _newDistance;
		}
	}
}

void AHandcarActor::Rotate(const float _deltaTime)
{
	if (currentRail && currentSpeed != 0.0f)
	{
		const FVector _actorLoc = GetActorLocation();

		const float _addedDistance = FMath::Abs(currentSpeed) * _deltaTime;
		float _lookAtDistance = currentDistance + _addedDistance;

		ARailActor* _lookAtRail = currentRail;

		if (_lookAtDistance >= currentRail->GetLength())
		{
			_lookAtRail = FindNextRail(false);

			if (_lookAtRail)
				_lookAtDistance = _lookAtRail->GetDistanceAtLocation(_actorLoc) + _addedDistance;
		}

		if (_lookAtRail)
		{
			const FRotator _actorRot = GetActorRotation();
			const FVector _currentTargetLoc = _lookAtRail->GetLocationAtDistance(_lookAtDistance);

			//Note : No lerp for the roll.
			FRotator _lookAt = UKismetMathLibrary::FindLookAtRotation(_actorLoc, _currentTargetLoc);
			_lookAt.Roll = _actorRot.Roll;

			const FRotator _rotation = FMath::Lerp(_actorRot, _lookAt, rotationSpeed * _deltaTime);
			SetActorRotation(_rotation);
		}
	}
}


void AHandcarActor::CheckUpdateIsBlocked(const float _distance)
{
	const float _stopDistance = currentRail->GetDistanceAtStopLocation(onReturn);
	const bool _updateIsBlocked = onReturn ? _distance <= _stopDistance
									: _distance >= _stopDistance;

	if (_updateIsBlocked)
	{
		nextRail = FindNextRail(true);
		isBlocked = nextRail ? false
					: true;

		currentSpeed = isBlocked ? 0.0f
						: currentSpeed;
	}
	else
		isBlocked = false;
}

bool AHandcarActor::CheckUpdateCurrentRail(const float _distance)
{
	const bool _updateRail = onReturn ? _distance <= 0.0f
								: _distance >= currentRail->GetLength();

	if (_updateRail)
	{
		if (!nextRail)
			nextRail = FindNextRail(true);

		if (nextRail)
		{
			currentRail = nextRail;
			return true;
		}
	}

	return false;
}

ARailActor* AHandcarActor::FindNextRail(const bool _isUnblocked) const
{
	if (currentRail)
	{
		ARailLeverActor* _nextLever = currentRail->GetLever(onReturn);

		if (_nextLever)
		{
			ARailActor* _nextRail = _nextLever->GetNextRail(onReturn);

			if (_isUnblocked)
			{
				if (!currentRail->GetIsBlocked(_nextLever))
				{
					if (_nextRail)
						if (!_nextRail->GetIsBlocked(_nextLever))
							return _nextRail;
				}
						
				_nextRail = nullptr;
			}
			
			return _nextRail;
		}
	}

	return nullptr;
}


#pragma region Speed
void AHandcarActor::UpdateCurrentSpeed(const float _deltaTime)
{
	const float _addedSpeed = totalAcceleration * _deltaTime;
	float _newSpeed = currentSpeed + _addedSpeed;
	CLAMP(_newSpeed, -maxSpeed, maxSpeed);

	if (totalAcceleration == currentFriction)
	{
		if (FMath::IsNearlyEqual(_newSpeed, 0.0f, FMath::Abs(_addedSpeed)))
			_newSpeed = 0.0f;
	}

	currentSpeed = _newSpeed;
}
#pragma endregion


#pragma region Acceleration
void AHandcarActor::UpdateCurrentFriction()
{
	if (currentSpeed == 0.0f)
	{
		currentFriction = 0.0f;
		return;
	}

	const float _friction = currentSpeed > 0.0f ? friction : -friction;
	currentFriction = _friction - (currentSpeed * frictionSpeedPercent);
}

void AHandcarActor::UpdateCurrentSlopeAcceleration()
{
	//The slope acceleration stays the same if the handcar isn't moving.
	if (slopeAccelerationCurve && currentSpeed != 0.0f)
	{
		//Note : The curve has different values for the climb and the descent. All the values are positives.
		//A positive slope angle gives a value for a climb and a negative gives a value for a descent.

		const float _slopeAngle = GetActorRotation().Pitch;

		//If the handcar is on the return, I take the slope acceleration value for the negated slope angle.
		//Because if the handcar is in a climb on the departure, I consider it's in a descent on the return. And vice versa.
		const float _newSlopeAcceleration = onReturn ? slopeAccelerationCurve->GetFloatValue(-_slopeAngle) : slopeAccelerationCurve->GetFloatValue(_slopeAngle);

		//I manually negates the acceleration value if the slope angle is positive.
		//Because a positive slope angle means a loss in the speed, no matter if the handcar is considered in a climb or a descent.
		currentSlopeAcceleration = _slopeAngle >= 0.0f ? -_newSlopeAcceleration : _newSlopeAcceleration;
	}
}

void AHandcarActor::UpdateTotalAcceleration()
{
	UpdateCurrentFriction();
	UpdateCurrentSlopeAcceleration();

	float _totalAcceleration = currentSlopeAcceleration + currentFriction;

	if (nitro)
		_totalAcceleration += nitro->GetCurrentAcceleration();

	if (handle)
		_totalAcceleration += handle->GetCurrentAcceleration();

	totalAcceleration = _totalAcceleration;
}
#pragma endregion


#pragma endregion


void AHandcarActor::BeginPlay()
{
	Super::BeginPlay();
	UpdatePropertiesFromDA();
	InitLocationRotation();

	if (nitro)
		nitro->NitroBeginPlay(dataAsset);

	if (handle)
		handle->HandleBeginPlay(dataAsset, this);
}

void AHandcarActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isRuntime)
	{
		if (nitro)
			nitro->NitroTick(DeltaTime);

		if (handle)
			handle->HandleTick(DeltaTime);

		UpdateTotalAcceleration();
		UpdateCurrentSpeed(DeltaTime);

		onReturn = currentSpeed < 0.0f ? true
					: false;

		Move(DeltaTime);
		Rotate(DeltaTime);
	}
}


void AHandcarActor::PrintDebug() const
{
	if (useSpeedDebug)
		PRINT_SCREEN_WITH_FLOAT_TICK("[Handcar] Speed : ", currentSpeed, FColor::Blue);

	if (useFrictionDebug)
		PRINT_SCREEN_WITH_FLOAT_TICK("[Handcar] Friction : ", currentFriction, FColor::Red);

	if (useSlopeAccelerationDebug)
		PRINT_SCREEN_WITH_FLOAT_TICK("[Handcar] Slope Acceleration : ", currentSlopeAcceleration, FColor::Magenta);

	if (useTotalAccelerationDebug)
		PRINT_SCREEN_WITH_FLOAT_TICK("[Handcar] Total Acceleration : ", totalAcceleration, FColor::Magenta);
}
