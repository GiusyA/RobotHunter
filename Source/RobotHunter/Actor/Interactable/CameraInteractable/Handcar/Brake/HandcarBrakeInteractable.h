#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "RobotHunter/Actor/Interactable/CameraInteractable/CameraInteractableActor.h"
#include "RobotHunter/DataAsset/Handcar/HandcarPropertiesDA.h"
#include "HandcarBrakeInteractable.generated.h"

class AHandcarActor;

UCLASS()
class ROBOTHUNTER_API AHandcarBrakeInteractable : public ACameraInteractableActor
{
	GENERATED_BODY()
	

#pragma region Properties


#pragma region Debug
	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug|Brake", meta = (EditCondition = "useDebug", EditConditionHides))
	bool usePositionOnAngleScopeDebug;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug|Brake", meta = (EditCondition = "useDebug", EditConditionHides))
	bool useDecelerationValueDebug;
#pragma endregion


#pragma region Position
	UPROPERTY(EditAnywhere, Category = "Custom Property|Position")
	TObjectPtr<USceneComponent> leverBase;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Position")
	TObjectPtr<UStaticMeshComponent> leverMesh;


#pragma region Angle
	UPROPERTY(EditAnywhere, Category = "Custom Property|Position|Angle", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 359.0f, ClampMax = 359.0f))
	float tightenedAngle;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Position|Angle", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 359.0f, ClampMax = 359.0f))
	float releasedAngle;
#pragma endregion


#pragma region Movement
	UPROPERTY(EditAnywhere, Category = "Custom Property|Position|Movement", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float tightenMinValue;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Position|Movement", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float tightenMaxValue;


	UPROPERTY(EditAnywhere, Category = "Custom Property|Position|Movement", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float releaseSpeed;
#pragma endregion


	bool isReleased;
	float positionOnAngleScope;
#pragma endregion


	UPROPERTY()
	TObjectPtr<AHandcarActor> handcar;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Deceleration", meta = (UIMax = 0.0f, ClampMax = 0.0f))
	float decelerationValue;

	float currentDecelerationValue;
#pragma endregion


#pragma region Setter/Getter
public:
	FORCEINLINE bool GetIsReleased() const { return isReleased; }
#pragma endregion


#pragma region Functions
public:
	AHandcarBrakeInteractable();

private:
	void TightenLever(const FInputActionValue& _value);
	void ReleaseLever(const FInputActionValue& _value);

	void ReleasingLever(const float _deltaTime);

	void UpdatePropertiesFromDA(UHandcarPropertiesDA* _da);
	void UpdatePositionOnAngleScope();

protected:
	virtual void PrintDebug() const override;

	virtual void SetupPlayerInputs(ACustomPlayer* _player) override;

	virtual void FirstInteraction(ACustomPlayer* _player, USceneComponent* _playerPosition) override;
	virtual void SecondInteraction(ACustomPlayer* _player) override;

public:
	void BrakeBeginPlay(UHandcarPropertiesDA* _da, AHandcarActor* _handcar);
	void BrakeTick(const float _deltaTime);

	float GetUpdateCurrentDecelerationValue(const float _handcarSpeed);
#pragma endregion
};
