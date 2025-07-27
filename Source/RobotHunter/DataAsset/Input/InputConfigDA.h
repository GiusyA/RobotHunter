#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputMappingContext.h"
#include "InputConfigDA.generated.h"

UCLASS()
class ROBOTHUNTER_API UInputConfigDA : public UDataAsset
{
	GENERATED_BODY()
	

#pragma region Properties


#pragma region Context
	UPROPERTY(EditAnywhere, Category = "Custom Property|Context")
	TObjectPtr<UInputMappingContext> gameContext;
#pragma endregion


#pragma region Action
	UPROPERTY(EditAnywhere, Category = "Custom Property|Action")
	TObjectPtr<UInputAction> moveAction;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Action")
	TObjectPtr<UInputAction> rotateCameraAction;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Action")
	TObjectPtr<UInputAction> jumpAction;


	UPROPERTY(EditAnywhere, Category = "Custom Property|Action")
	TObjectPtr<UInputAction> interactionAction;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Action")
	TObjectPtr<UInputAction> aimAction;


#pragma region Handcar
	UPROPERTY(EditAnywhere, Category = "Custom Property|Action|Handcar")
	TObjectPtr<UInputAction> handcarNitroCursorAction; 

	
#pragma region Brake
	UPROPERTY(EditAnywhere, Category = "Custom Property|Action|Handcar")
	TObjectPtr<UInputAction> handcarBrakeTightenAction;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Action|Handcar")
	TObjectPtr<UInputAction> handcarBrakeReleaseAction;
#pragma endregion


#pragma region Handle
	UPROPERTY(EditAnywhere, Category = "Custom Property|Action|Handcar")
	TObjectPtr<UInputAction> handcarHandleAcceleration;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Action|Handcar")
	TObjectPtr<UInputAction> handcarHandleChangeWay;
#pragma endregion


#pragma endregion


#pragma endregion


#pragma endregion


#pragma region Setter/Getter
public:
#pragma region Context
	FORCEINLINE UInputMappingContext* GetGameContext() const { return gameContext; }
#pragma endregion

#pragma region Action
	FORCEINLINE UInputAction* GetMoveAction() const { return moveAction; }
	FORCEINLINE UInputAction* GetRotateCameraAction() const { return rotateCameraAction; }
	FORCEINLINE UInputAction* GetJumpAction() const { return jumpAction; }

	FORCEINLINE UInputAction* GetInteractionAction() const { return interactionAction; }

	FORCEINLINE UInputAction* GetAimAction() const { return aimAction; }

#pragma region Handcar
	FORCEINLINE UInputAction* GetHandcarNitroCursorAction() const { return handcarNitroCursorAction; }

#pragma region Brake
	FORCEINLINE UInputAction* GetHandcarBrakeTightenAction() const { return handcarBrakeTightenAction; }
	FORCEINLINE UInputAction* GetHandcarBrakeReleaseAction() const { return handcarBrakeReleaseAction; }
#pragma endregion

#pragma region Handle
	FORCEINLINE UInputAction* GetHandcarHandleAcceleration() const { return handcarHandleAcceleration; }
	FORCEINLINE UInputAction* GetHandcarHandleChangeWay() const { return handcarHandleChangeWay; }
#pragma endregion

#pragma endregion

#pragma endregion

#pragma endregion


#pragma region Functions
public:
	UInputConfigDA();
#pragma endregion
};
