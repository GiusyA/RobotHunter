#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/FSM/State/CustomState.h"
#include "InputMappingContext.h"
#include "CustomPlayerState.generated.h"

class ACustomPlayer;

UCLASS()
class ROBOTHUNTER_API UCustomPlayerState : public UCustomState
{
	GENERATED_BODY()


#pragma region Properties
protected:
	UPROPERTY(EditAnywhere, Category = "Custom Property|Camera")
	TEnumAsByte<ECameraKey> cameraSettingsKey;


	UPROPERTY(EditAnywhere, Category = "Custom Property|UI")
	TArray<TEnumAsByte<EWidgetType>> widgetTypes;


	UPROPERTY(EditAnywhere, Category = "Custom Property|Input")
	TObjectPtr<UInputMappingContext> inputContext;


	UPROPERTY()
	TObjectPtr<ACustomPlayer> player;

	UPROPERTY()
	TObjectPtr<ACustomHUD> hud;
#pragma endregion


#pragma region Functions
public:
	UCustomPlayerState();

protected:
	void RetrievePlayer();
	void RetrieveHUD();

	void UpdateCameraSettings();
	void ShowWidgets();
	void HideWidgets();

	void DisablePlayerInputContext();
	UFUNCTION() void EnablePlayerInputContext();

public:
	virtual void Enter() override;
	virtual void Exit() override;
#pragma endregion
};
