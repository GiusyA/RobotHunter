#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Actor/CustomActor.h"
#include "RobotHunter/DataAsset/Handcar/HandcarPropertiesDA.h"
#include "RobotHunter/Actor/Spline/Rail/RailActor.h"
#include "RobotHunter/Actor/Interactable/CameraInteractable/Handcar/Nitro/HandcarNitroInteractable.h"
#include "RobotHunter/Actor/Interactable/CameraInteractable/Handcar/Brake/HandcarBrakeInteractable.h"
#include "RobotHunter/Actor/Interactable/DoubleInteractable/Handcar/Handle/HandcarHandleInteractable.h"
#include "Components/BoxComponent.h"
#include "HandcarActor.generated.h"

UCLASS()
class ROBOTHUNTER_API AHandcarActor : public ACustomActor
{
	GENERATED_BODY()

#pragma region Properties


#pragma region DataAsset
	UPROPERTY(EditAnywhere, Category = "Custom Property|Data Asset")
	bool useDataAsset;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Data Asset")
	TObjectPtr<UHandcarPropertiesDA> dataAsset;
#pragma endregion


#pragma region Debug
	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug|Handcar", meta = (EditCondition = "useDebug", EditConditionHides))
	bool useSpeedDebug;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug|Handcar", meta = (EditCondition = "useDebug", EditConditionHides))
	bool useFrictionDebug;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug|Handcar", meta = (EditCondition = "useDebug", EditConditionHides))
	bool useSlopeAccelerationDebug;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug|Handcar", meta = (EditCondition = "useDebug", EditConditionHides))
	bool useTotalAccelerationDebug;
#pragma endregion


#pragma region Movement
	UPROPERTY(EditAnywhere, Category = "Custom Property|Movement")
	TObjectPtr<ARailActor> currentRail;

	UPROPERTY()
	TObjectPtr<ARailActor> nextRail;


#pragma region Speed
	UPROPERTY(EditAnywhere, Category = "Custom Property|Movement|Speed", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float maxSpeed;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Movement|Speed", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float rotationSpeed;

	float currentSpeed;
#pragma endregion


#pragma region Acceleration


#pragma region Friction
	UPROPERTY(EditAnywhere, Category = "Custom Property|Movement|Acceleration|Friction", meta = (UIMax = -0.1f, ClampMax = -0.1f))
	float friction;

	/// <summary>
	/// Multiplier applied to the substracted current speed from the friction.
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Custom Property|Movement|Acceleration|Friction", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 1.0f, ClampMax = 1.0f))
	float frictionSpeedPercent;

	float currentFriction;
#pragma endregion


#pragma region Slope
	UPROPERTY(EditAnywhere, Category = "Custom Property|Movement|Acceleration|Slope")
	TObjectPtr<UCurveFloat> slopeAccelerationCurve;

	float currentSlopeAcceleration;
#pragma endregion


	float totalAcceleration;
#pragma endregion


	bool isBlocked;
	bool onReturn;


	float currentDistance;
#pragma endregion


	UPROPERTY(EditAnywhere, Category = "Custom Property|Nitro")
	TObjectPtr<AHandcarNitroInteractable> nitro;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Brake")
	TObjectPtr<AHandcarBrakeInteractable> brake;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Handle")
	TObjectPtr<AHandcarHandleInteractable> handle;
#pragma endregion


#pragma region Setter/Getter
public:
	FORCEINLINE bool GetIsBlocked() const { return isBlocked; }
	FORCEINLINE bool GetOnReturn() const { return onReturn; }

	FORCEINLINE float GetCurrentSpeed() const { return currentSpeed; }
	FORCEINLINE float GetMaxSpeed() const { return maxSpeed; }
	FORCEINLINE float GetTotalAcceleration() const { return totalAcceleration; }
	FORCEINLINE float GetCurrentDistance() const { return currentDistance; }

	FORCEINLINE ARailActor* GetCurrentRail() const { return currentRail; }

	FORCEINLINE AHandcarNitroInteractable* GetNitro() const { return nitro; }
	FORCEINLINE AHandcarBrakeInteractable* GetBrake() const { return brake; }
	FORCEINLINE AHandcarHandleInteractable* GetHandle() const { return handle; }
#pragma endregion


#pragma region Functions
public:
	AHandcarActor();

private:
	void InitLocationRotation();
	void UpdatePropertiesFromDA();

#pragma region Movement
	void Move(const float _deltaTime);
	void Rotate(const float _deltaTime);

	void CheckUpdateIsBlocked(const float _distance);
	bool CheckUpdateCurrentRail(const float _distance);
	ARailActor* FindNextRail(const bool _isUnblocked) const;

#pragma region Speed
	void UpdateCurrentSpeed(const float _deltaTime);
#pragma endregion

#pragma region Acceleration
	void UpdateCurrentFriction();
	void UpdateCurrentSlopeAcceleration();
	void UpdateTotalAcceleration();
#pragma endregion

#pragma endregion

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void PrintDebug() const override;

	virtual void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:
	void ChangeWay();
#pragma endregion
};
