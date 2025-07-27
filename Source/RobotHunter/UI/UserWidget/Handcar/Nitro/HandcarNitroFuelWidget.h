#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/UserWidget/CustomUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "HandcarNitroFuelWidget.generated.h"

UCLASS()
class ROBOTHUNTER_API UHandcarNitroFuelWidget : public UCustomUserWidget
{
	GENERATED_BODY()
	

#pragma region Properties
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UProgressBar> fuelProgressBar;


	UPROPERTY(meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextBlock> maxFuelTextBlock;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextBlock> currentFuelTextBlock;


	float maxFuel;
	float currentFuel;
#pragma endregion


#pragma region Functions
public:
	UHandcarNitroFuelWidget(const FObjectInitializer& ObjectInitializer);

private:
	void UpdateFuelProgressBarPercent();

public:
	void SetMaxFuel(const float _maxFuel);
	void SetCurrentFuel(const float _currentFuel);
#pragma endregion
};
