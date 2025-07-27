#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Component/CustomActorComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "RobotHunter/Utils/Lerp/CustomLerp.h"
#include "CustomCameraComponent.generated.h"


#pragma region Struct
USTRUCT()
struct FCameraLagSettings
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere)
	bool enable;

	UPROPERTY(EditAnywhere)
	bool enableRotation;


	UPROPERTY(EditAnywhere, meta = (UIMin = 0.0f, ClampMin = 0.0f, EditCondition = "enable", EditConditionHides))
	float speed;

	UPROPERTY(EditAnywhere, meta = (UIMin = 0.0f, ClampMin = 0.0f, EditCondition = "enableRotation", EditConditionHides))
	float rotationSpeed;

	UPROPERTY(EditAnywhere, meta = (UIMin = 0.0f, ClampMin = 0.0f, EditCondition = "enable || enableRotation", EditConditionHides))
	float maxDistance;


	FCameraLagSettings()
	{
		enable = false;
		enableRotation = false;

		speed = 10.0f;
		rotationSpeed = 10.0f;
		maxDistance = 0.0f;
	}
};


USTRUCT()
struct FCameraTransitionSettings
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere)
	bool reverseCurve;

	UPROPERTY(EditAnywhere, meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float duration;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> curve;


	FCameraTransitionSettings()
	{
		reverseCurve = false;
		duration = 1.0f;
		curve = nullptr;
	}
};


USTRUCT()
struct FCameraSettings
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere)
	FVector position;


	UPROPERTY()
	bool setRotation;

	UPROPERTY(EditAnywhere, Category = "Rotation", meta = (EditCondition = "setRotation", EditConditionHides))
	FRotator rotation;

	UPROPERTY(EditAnywhere, Category = "Rotation", meta = (UIMin = -360.0f, ClampMin = -360.0f, UIMax = 0.0f, ClampMax = 0.0f, EditCondition = "!setRotation", EditConditionHides))
	float pitchClampMin;

	UPROPERTY(EditAnywhere, Category = "Rotation", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 360.0f, ClampMax = 360.0f, EditCondition = "!setRotation", EditConditionHides))
	float pitchClampMax;


	UPROPERTY(EditAnywhere, meta = (UIMin = 5.0f, ClampMin = 5.0f, UIMax = 170.0f, ClampMax = 170.0f))
	float fieldOfView;


	UPROPERTY(EditAnywhere)
	FCameraLagSettings lag;


	UPROPERTY(EditAnywhere)
	FCameraTransitionSettings defaultTransition;

	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<ECameraKey>, FCameraTransitionSettings> transitions;


	FCameraSettings()
	{
		position = FVector(0.0f);
		position.X = 300.0f;

		setRotation = false;
		rotation = FRotator(0.0f);
		pitchClampMin = -65.0f;
		pitchClampMax = 65.0f;

		fieldOfView = 90.0f;

		lag = FCameraLagSettings();

		defaultTransition = FCameraTransitionSettings();
		transitions = TMap<TEnumAsByte<ECameraKey>, FCameraTransitionSettings>();
	}


	FCameraTransitionSettings GetTransitionSettings(const ECameraKey& _key) const
	{
		if (transitions.Contains(_key))
			return transitions[_key];

		return defaultTransition;
	}

	float GetTransitionDuration(const ECameraKey& _key) const
	{
		return GetTransitionSettings(_key).duration;
	}
};
#pragma endregion


UCLASS()
class ROBOTHUNTER_API UCustomCameraComponent : public UCustomActorComponent
{
	GENERATED_BODY()


#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Custom Property")
	TObjectPtr<USpringArmComponent> springArmComponent;

	UPROPERTY(EditAnywhere, Category = "Custom Property")
	TObjectPtr<UCameraComponent> cameraComponent;


	UPROPERTY(EditAnywhere, Category = "Custom Property|Settings")
	TEnumAsByte<ECameraKey> currentSettingsKey;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Settings")
	TMap<TEnumAsByte<ECameraKey>, FCameraSettings> allSettings;


#pragma region Lerp
	bool canLerp;

	FCustomLerp socketOffsetLerp;
	FCustomLerp armLengthLerp;

	FCustomLerp rotationLerp;
#pragma endregion


	FCameraSettings currentSettings;
#pragma endregion


#pragma region Setter/Getter
public:
	FORCEINLINE float GetTransitionDuration(const ECameraKey& _key) const { return currentSettings.GetTransitionDuration(_key); }

	FORCEINLINE TEnumAsByte<ECameraKey> GetCurrentSettingsKey() const { return currentSettingsKey; }
	FORCEINLINE FCameraSettings GetCurrentSettings() const { return currentSettings; }


	FORCEINLINE FVector GetForwardVector() const
	{
		return springArmComponent ? springArmComponent->GetForwardVector()
				: FVector(0.0f);
	}

	FORCEINLINE FVector GetRightVector() const
	{
		return springArmComponent ? springArmComponent->GetRightVector()
				: FVector(0.0f);
	}

	FORCEINLINE FRotator GetRelativeRotation() const
	{
		return springArmComponent ? springArmComponent->GetRelativeRotation()
				: FRotator(0.0f);
	}


	FORCEINLINE USpringArmComponent* GetSpringArmComponent() const { return springArmComponent; }
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return cameraComponent; }
#pragma endregion


#pragma region Functions
public:
	UCustomCameraComponent();

private:
#pragma region Update
	void UpdateLag(const FCameraLagSettings& _newLagSettings);
	void UpdateFOV(const float _newFOV);
	void UpdatePosition(const FVector& _newPosition);
	void UpdateRotation(const FRotator& _newRotation);
#pragma endregion

#pragma region Lerp
	void InitSocketOffsetLerp();
	void InitArmLengthLerp();
	void InitRotationLerp();

	void StartLerps(const ECameraKey& _lastSettingsKey);

	void LerpPosition(const float _deltaTime);
	void LerpRotation(const float _deltaTime);
#pragma endregion

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	virtual void SetupAttachment(USceneComponent* _root) const override;

	void UpdateCurrentSettings(const ECameraKey& _key);

	void AddRelativeRotation(FRotator& _rotation);
	void SetRelativeRotation(FRotator& _rotation, const bool _updateYawOnly = true);
#pragma endregion
};
