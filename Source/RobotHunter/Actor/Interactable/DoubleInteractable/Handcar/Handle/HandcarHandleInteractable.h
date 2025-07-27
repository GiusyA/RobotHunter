#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "RobotHunter/Actor/Interactable/DoubleInteractable/DoubleInteractableActor.h"
#include "RobotHunter/DataAsset/Handcar/HandcarPropertiesDA.h"
#include "HandcarHandleInteractable.generated.h"

class AHandcarActor;

UCLASS()
class ROBOTHUNTER_API AHandcarHandleInteractable : public ADoubleInteractableActor
{
	GENERATED_BODY()
	

#pragma region Properties


#pragma region Debug
	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug|Handle", meta = (EditCondition = "useDebug", EditConditionHides))
	bool useAccelerationDebug;


	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug|Handle", meta = (EditCondition = "useDebug", EditConditionHides))
	bool useFirstHeightRapportDebug;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug|Handle", meta = (EditCondition = "useDebug", EditConditionHides))
	bool useSecondHeightRapportDebug;
#pragma endregion


#pragma region Mesh
	UPROPERTY(EditAnywhere, Category = "Custom Property|Mesh")
	TObjectPtr<UStaticMeshComponent> handleMesh;


#pragma region Rotation
	UPROPERTY(EditAnywhere, Category = "Custom Property|Mesh|Rotation", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 360.0f, ClampMax = 360.0f))
	float maxRotation;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Mesh|Rotation", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float rotationValue;
#pragma endregion


#pragma region Height
	float currentFirstHeightRapport;
	float currentSecondHeightRapport;

	float latestFirstHeightRapport;
	float latestSecondHeightRapport;
#pragma endregion


#pragma endregion


#pragma region Acceleration
	UPROPERTY(EditAnywhere, Category = "Custom Property|Acceleration", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float minForce;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Acceleration", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float maxForce;


	UPROPERTY(EditAnywhere, Category = "Custom Property|Acceleration", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float accelerationBoost;


	bool firstPlayerAccelerates;
	bool secondPlayerAccelerates;

	bool negateAcceleration;

	float currentAcceleration;
#pragma endregion


	UPROPERTY()
	TObjectPtr<AHandcarActor> handcar;
#pragma endregion


#pragma region Setter/Getter
public:
	FORCEINLINE void InverseNegateAcceleration() { negateAcceleration = !negateAcceleration; }

	FORCEINLINE float GetCurrentAcceleration() const { return currentAcceleration; }
#pragma endregion


#pragma region Functions
public:
	AHandcarHandleInteractable();

private:
	void UpdatePropertiesFromDA(UHandcarPropertiesDA* _da);

	void RotateHandle(const float _currentMaxRotation, const float _rotationValue, const float _deltaTime);
	void UpdateHandleHeightRapports();

	void UpdateCurrentAcceleration(const float _lastHeightRapport, const float _currentHeightRapport);
	void AddAccelerationBoost(const float _lowestHeightRapport, const float _highestHeightRapport);

	void StartAcceleration(const FInputActionValue& _value);
	void StopAcceleration(const FInputActionValue& _value);

	void ChangeWay(const FInputActionValue& _value);

protected:
	virtual void PrintDebug() const override;

	virtual void SetupPlayerInputs(ACustomPlayer* _player) override;

	virtual void FirstInteraction(ACustomPlayer* _player, USceneComponent* _playerPosition) override;
	virtual void SecondInteraction(ACustomPlayer* _player) override;

public:
	void HandleBeginPlay(UHandcarPropertiesDA* _da, AHandcarActor* _handcar);
	void HandleTick(const float _deltaTime);
#pragma endregion
};
