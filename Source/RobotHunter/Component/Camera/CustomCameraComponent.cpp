#include "CustomCameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

UCustomCameraComponent::UCustomCameraComponent()
{
	springArmComponent = CREATE_DEFAULT_SUBOBJECT(USpringArmComponent, "SpringArmComponent");
	cameraComponent = CREATE_DEFAULT_SUBOBJECT(UCameraComponent, "CameraComponent");

	currentSettingsKey = FString();
	allSettings = TMap<FString, FCameraSettings>();

#pragma region Lerp
	useLerp = false;

	isLerpingPosition = false;
	isLerpingRotation = false;
#pragma endregion

	currentSettings = FCameraSettings();
}


void UCustomCameraComponent::ClampPitch(FRotator& _rotation)
{
	
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
void UCustomCameraComponent::LerpPosition(const float _lerpAlpha, const FVector& _newPosition)
{
	if (isLerpingPosition && springArmComponent)
	{
		float& _armLength = springArmComponent->TargetArmLength;
		_armLength = UKismetMathLibrary::Lerp(_armLength, _newPosition.X, _lerpAlpha);

		FVector& _socketOffset = springArmComponent->SocketOffset;
		const FVector _newSocketOffset = FVector(_socketOffset.X, _newPosition.Y, _newPosition.Z);
		_socketOffset = UKismetMathLibrary::VLerp(_socketOffset, _newSocketOffset, _lerpAlpha);

		const bool _armLengthDone = FMath::IsNearlyEqual(_armLength, _newPosition.X, 0.1f);
		const bool _offsetDone = _socketOffset.Equals(_newSocketOffset, 0.1f);
		isLerpingPosition = !(_armLengthDone && _offsetDone);

		if (!isLerpingPosition)
		{
			_armLength = _newPosition.X;
			_socketOffset = _newSocketOffset;
		}
	}
}

void UCustomCameraComponent::LerpRotation(const float _lerpAlpha, const FRotator& _newRotation)
{
	if (currentSettings.setRotation && isLerpingRotation)
	{
		if (springArmComponent)
		{
			const FRotator _rotation = springArmComponent->GetRelativeRotation();
			const FRotator _lerpedRotation = UKismetMathLibrary::RLerp(_rotation, _newRotation, _lerpAlpha, true);
			springArmComponent->SetRelativeRotation(_lerpedRotation);

			isLerpingRotation = !(_lerpedRotation.Equals(_newRotation, 0.1f));

			if (!isLerpingRotation)
				springArmComponent->SetRelativeRotation(_newRotation);
		}
	}
}
#pragma endregion


void UCustomCameraComponent::BeginPlay()
{
	Super::BeginPlay();
	useLerp = true;
}

void UCustomCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (isRuntime)
	{
		const float _lerpAlpha = DeltaTime / currentSettings.blendSpeed;

		LerpPosition(_lerpAlpha, currentSettings.position);
		LerpRotation(_lerpAlpha, currentSettings.rotation);
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


void UCustomCameraComponent::UpdateCurrentSettings(const FString& _key)
{
	if (allSettings.Contains(_key))
	{
		currentSettingsKey = _key;
		currentSettings = allSettings[_key];

		UpdateLag(currentSettings.lag);
		UpdateFOV(currentSettings.fieldOfView);

		if (useLerp)
		{
			isLerpingPosition = isLerpingRotation = true;
			return;
		}

		UpdatePosition(currentSettings.position);
		UpdateRotation(currentSettings.rotation);
	}
}

void UCustomCameraComponent::SetupAttachment(USceneComponent* _root) const
{
	SETUP_ATTACHMENT(springArmComponent, _root);
	SETUP_ATTACHMENT(cameraComponent, springArmComponent);
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
