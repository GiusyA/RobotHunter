#include "CustomCameraComponent.h"

UCustomCameraComponent::UCustomCameraComponent()
{
	springArmComponent = CREATE_DEFAULT_SUBOBJECT(USpringArmComponent, "SpringArmComponent");
	cameraComponent = CREATE_DEFAULT_SUBOBJECT(UCameraComponent, "CameraComponent");

	currentSettingsKey = ECameraKey::DefaultCameraKey;
	allSettings = TMap<TEnumAsByte<ECameraKey>, FCameraSettings>();

#pragma region Lerp
	canLerp = false;

	socketOffsetLerp = FCustomLerp(ECustomLerpType::VectorLerpType);
	armLengthLerp = FCustomLerp(ECustomLerpType::FloatLerpType);

	rotationLerp = FCustomLerp(ECustomLerpType::RotatorLerpType);
#pragma endregion

	currentSettings = FCameraSettings();
}


#pragma region Update
void UCustomCameraComponent::UpdateLag(const FCameraLagSettings& _newLagSettings)
{
	if (springArmComponent)
	{
		springArmComponent->bEnableCameraLag = _newLagSettings.enable;
		springArmComponent->bEnableCameraRotationLag = _newLagSettings.enableRotation;
		springArmComponent->CameraLagSpeed = _newLagSettings.speed;
		springArmComponent->CameraRotationLagSpeed = _newLagSettings.rotationSpeed;
		springArmComponent->CameraLagMaxDistance = _newLagSettings.maxDistance;
	}
}

void UCustomCameraComponent::UpdateFOV(const float _newFOV)
{
	if (cameraComponent)
		cameraComponent->FieldOfView = _newFOV;
}

void UCustomCameraComponent::UpdatePosition(const FVector& _newPosition)
{
	if (springArmComponent)
	{
		springArmComponent->TargetArmLength = _newPosition.X;

		FVector& _socketOffset = springArmComponent->SocketOffset;
		const FVector _newSocketOffset = FVector(_socketOffset.X, _newPosition.Y, _newPosition.Z);
		_socketOffset = _newSocketOffset;
	}
}

void UCustomCameraComponent::UpdateRotation(const FRotator& _newRotation)
{
	if (currentSettings.setRotation)
		if (springArmComponent)
			springArmComponent->SetRelativeRotation(_newRotation);
}
#pragma endregion


#pragma region Lerp
void UCustomCameraComponent::InitSocketOffsetLerp()
{
	if (springArmComponent)
	{
		const FVector _socketOffset = springArmComponent->SocketOffset;
		socketOffsetLerp.baseValueVector = _socketOffset;
		socketOffsetLerp.finalValueVector = FVector(_socketOffset.X, currentSettings.position.Y, currentSettings.position.Z);
	}
}

void UCustomCameraComponent::InitArmLengthLerp()
{
	if (springArmComponent)
	{
		armLengthLerp.baseValueFloat = springArmComponent->TargetArmLength;
		armLengthLerp.finalValueFloat = currentSettings.position.X;
	}
}

void UCustomCameraComponent::InitRotationLerp()
{
	if (springArmComponent)
	{
		rotationLerp.baseValueRotator = springArmComponent->GetRelativeRotation();
		rotationLerp.finalValueRotator = currentSettings.rotation;
	}
}


void UCustomCameraComponent::StartLerps(const ECameraKey& _lastSettingsKey)
{
	InitSocketOffsetLerp();
	InitArmLengthLerp();
	InitRotationLerp();

	const FCameraTransitionSettings _transitionSettings = currentSettings.GetTransitionSettings(_lastSettingsKey);

	socketOffsetLerp.curve = armLengthLerp.curve = rotationLerp.curve = _transitionSettings.curve;
	socketOffsetLerp.reverseCurve = armLengthLerp.reverseCurve = rotationLerp.reverseCurve = _transitionSettings.reverseCurve;
	socketOffsetLerp.totalDuration = armLengthLerp.totalDuration = rotationLerp.totalDuration = _transitionSettings.duration;

	socketOffsetLerp.StartLerp();
	armLengthLerp.StartLerp();
	rotationLerp.StartLerp();
}


void UCustomCameraComponent::LerpPosition(const float _deltaTime)
{
	if (canLerp && springArmComponent)
	{
		if (socketOffsetLerp.GetIsLerping())
		{
			socketOffsetLerp.LerpValue(_deltaTime);
			springArmComponent->SocketOffset = socketOffsetLerp.GetCurrentValueVector();
		}

		if (armLengthLerp.GetIsLerping())
		{
			armLengthLerp.LerpValue(_deltaTime);
			springArmComponent->TargetArmLength = armLengthLerp.GetCurrentValueFloat();
		}
	}
}

void UCustomCameraComponent::LerpRotation(const float _deltaTime)
{
	if (canLerp && currentSettings.setRotation && springArmComponent)
	{
		if (rotationLerp.GetIsLerping())
		{
			rotationLerp.LerpValue(_deltaTime);
			const FRotator _newRotation = rotationLerp.GetCurrentValueRotator();
			springArmComponent->SetRelativeRotation(_newRotation);
		}
	}
}
#pragma endregion


void UCustomCameraComponent::BeginPlay()
{
	Super::BeginPlay();
	canLerp = true;
}

void UCustomCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (isRuntime)
	{
		LerpPosition(DeltaTime);
		LerpRotation(DeltaTime);
	}
}


#if WITH_EDITOR
void UCustomCameraComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	const bool _canUpdateSettings = PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UCustomCameraComponent, currentSettingsKey)
		|| PropertyChangedEvent.MemberProperty->GetName() == GET_MEMBER_NAME_CHECKED(UCustomCameraComponent, allSettings);

	if (_canUpdateSettings)
		UpdateCurrentSettings(currentSettingsKey);

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif


void UCustomCameraComponent::SetupAttachment(USceneComponent* _root) const
{
	SETUP_ATTACHMENT(springArmComponent, _root);
	SETUP_ATTACHMENT(cameraComponent, springArmComponent);
}


void UCustomCameraComponent::UpdateCurrentSettings(const ECameraKey& _key)
{
	if (allSettings.Contains(_key))
	{
		const ECameraKey _lastSettingsKey = currentSettingsKey;
		currentSettingsKey = _key;
		currentSettings = allSettings[_key];

		UpdateLag(currentSettings.lag);
		UpdateFOV(currentSettings.fieldOfView);

		if (canLerp)
		{
			StartLerps(_lastSettingsKey);
			return;
		}

		UpdatePosition(currentSettings.position);
		UpdateRotation(currentSettings.rotation);
	}
}


void UCustomCameraComponent::AddRelativeRotation(FRotator& _rotation)
{
	if (!currentSettings.setRotation)
	{
		if (springArmComponent)
		{
			const float _currentPitch = springArmComponent->GetRelativeRotation().Pitch;
			double& _rotationPitch = _rotation.Pitch;

			//For Unreal rotation pitch, up is in the negatives and down is in the positives.
			//I think the opposite is easier so I inverse the values here because I don't want it to be confusing in the blueprint.
			const float _pitchClamp = _rotationPitch < 0.0f ? -currentSettings.pitchClampMax
				: -currentSettings.pitchClampMin;

			const float _pitchClampDifference = _pitchClamp - _currentPitch;

			_rotationPitch = FMath::Abs(_rotationPitch) > FMath::Abs(_pitchClampDifference) ? _pitchClampDifference
				: _rotationPitch;

			springArmComponent->AddRelativeRotation(_rotation);
		}
	}
}

void UCustomCameraComponent::SetRelativeRotation(FRotator& _rotation, const bool _updateYawOnly)
{
	if (springArmComponent)
	{
		if (_updateYawOnly)
		{
			const FRotator _currentRot = springArmComponent->GetRelativeRotation();
			_rotation.Roll = _currentRot.Roll;
			_rotation.Pitch = _currentRot.Pitch;
		}
		else
		{
			//For Unreal rotation pitch, up is in the negatives and down is in the positives.
			//I think the opposite is easier so I inverse the values here because I don't want it to be confusing in the blueprint.
			CLAMP(_rotation.Pitch, -currentSettings.pitchClampMax, -currentSettings.pitchClampMin);
		}

		springArmComponent->SetRelativeRotation(_rotation);
	}
}
