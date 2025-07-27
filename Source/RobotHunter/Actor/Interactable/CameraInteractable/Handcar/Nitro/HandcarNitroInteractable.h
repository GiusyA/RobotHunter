#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "RobotHunter/Actor/Interactable/CameraInteractable/CameraInteractableActor.h"
#include "RobotHunter/UI/UserWidget/Handcar/Nitro/HandcarNitroWidget.h"
#include "RobotHunter/UI/UserWidget/Handcar/Nitro/HandcarNitroFuelWidget.h"
#include "RobotHunter/DataAsset/Handcar/HandcarPropertiesDA.h"
#include "HandcarNitroInteractable.generated.h"

class AHandcarActor;

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


#pragma region Fuel
	UPROPERTY(EditAnywhere, Category = "Custom Property|Fuel", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float maxFuel;

	bool hasFuel;
	float currentFuel;


#pragma region Consumption
	UPROPERTY(EditAnywhere, Category = "Custom Property|Fuel|Consumption", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float greenZoneFuelConsumption;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Fuel|Consumption", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float yellowZoneFuelConsumption;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Fuel|Consumption", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float redZoneFuelConsumption;

	float currentFuelConsumption;
#pragma endregion


#pragma endregion


#pragma region UI
	UPROPERTY()
	TObjectPtr<UHandcarNitroWidget> widget;

	UPROPERTY()
	TObjectPtr<UHandcarNitroFuelWidget> fuelWidget;
#pragma endregion


	UPROPERTY()
	TObjectPtr<UHandcarPropertiesDA> propertiesDA;

	UPROPERTY()
	TObjectPtr<AHandcarActor> handcar;


	bool isActive;
	float currentAcceleration;

	bool negateAcceleration;
#pragma endregion


#pragma region Setter/Getter
public:
	FORCEINLINE void InverseNegateAcceleration() { negateAcceleration = !negateAcceleration; }

	FORCEINLINE bool GetIsActive() const { return isActive; }
	FORCEINLINE float GetCurrentAcceleration() const { return currentAcceleration; }
#pragma endregion


#pragma region Functions
public:
	AHandcarNitroInteractable();

private:
	void InitializeWidget();
	void InitializeFuelWidget();
	void MoveWidgetCursor(const FInputActionValue& _value);

	void UpdatePropertiesFromDA();
	void UpdateCurrentFuelConsumption();
	void UpdateCurrentAcceleration();

	void ConsumeFuel(const float _deltaTime);

	UHandcarNitroWidget* FindHandcarNitroWidget() const;
	UHandcarNitroFuelWidget* FindHandcarNitroFuelWidget() const;

protected:
	virtual void PrintDebug() const override;

	virtual void SetupPlayerInputs(ACustomPlayer* _player) override;

	virtual void FirstInteraction(ACustomPlayer* _player, USceneComponent* _playerPosition) override;
	virtual void SecondInteraction(ACustomPlayer* _player) override;

public:
	void NitroBeginPlay(UHandcarPropertiesDA* _da, AHandcarActor* _handcar);
	void NitroTick(const float _deltaTime);
#pragma endregion
};
