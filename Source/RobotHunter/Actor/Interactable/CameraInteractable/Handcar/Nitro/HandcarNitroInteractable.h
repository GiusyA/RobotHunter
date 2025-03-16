#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "RobotHunter/Actor/Interactable/CameraInteractable/CameraInteractableActor.h"
#include "RobotHunter/UI/UserWidget/Handcar/Nitro/HandcarNitroWidget.h"
#include "RobotHunter/DataAsset/Handcar/HandcarPropertiesDA.h"
#include "HandcarNitroInteractable.generated.h"


UCLASS()
class ROBOTHUNTER_API AHandcarNitroInteractable : public ACameraInteractableActor
{
	GENERATED_BODY()
	

#pragma region Properties


#pragma region Debug
	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug|Nitro", meta = (EditCondition = "useDebug", EditConditionHides))
	bool useAccelerationDebug;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug|Nitro", meta = (EditCondition = "useDebug", EditConditionHides))
	bool useFuelConsumptionDebug;
#pragma endregion


	UPROPERTY(EditAnywhere, Category = "Custom Property|Mesh")
	TObjectPtr<UStaticMeshComponent> valveMesh;


	UPROPERTY(EditAnywhere, Category = "Custom Property|Acceleration", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float acceleration;


#pragma region FuelConsumption
	UPROPERTY(EditAnywhere, Category = "Custom Property|Fuel Consumption", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float greenZoneFuelConsumption;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Fuel Consumption", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float yellowZoneFuelConsumption;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Fuel Consumption", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float redZoneFuelConsumption;

	float currentFuelConsumption;
#pragma endregion


	UPROPERTY()
	TObjectPtr<UHandcarPropertiesDA> propertiesDA;

	UPROPERTY()
	TObjectPtr<UHandcarNitroWidget> widget;


	bool isActive;
	float currentAcceleration;

	bool negateAcceleration;
#pragma endregion


#pragma region Setter/Getter
public:
	FORCEINLINE void SetNegateAcceleration(const bool _negate) { negateAcceleration = _negate; }

	FORCEINLINE bool GetIsActive() const { return isActive; }
	FORCEINLINE float GetCurrentAcceleration() const { return currentAcceleration; }
#pragma endregion


#pragma region Functions
public:
	AHandcarNitroInteractable();

private:
	void InitializeWidget();
	void MoveWidgetCursor(const FInputActionValue& _value);

	void UpdatePropertiesFromDA();
	void UpdateCurrentFuelConsumption();
	void UpdateCurrentAcceleration();

	UHandcarNitroWidget* FindHandcarNitroWidget() const;

protected:
	virtual void PrintDebug() const override;

	virtual void SetupPlayerInputs(ACustomPlayer* _player) override;

	virtual void FirstInteraction(ACustomPlayer* _player, USceneComponent* _playerPosition) override;
	virtual void SecondInteraction(ACustomPlayer* _player) override;

public:
	void NitroBeginPlay(UHandcarPropertiesDA* _da);
	void NitroTick(const float _deltaTime);
#pragma endregion
};
