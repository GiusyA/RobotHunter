#include "HandcarNitroFuelWidget.h"

UHandcarNitroFuelWidget::UHandcarNitroFuelWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	fuelProgressBar = nullptr;

	maxFuelTextBlock = nullptr;
	currentFuelTextBlock = nullptr;

	maxFuel = 0.0f;
	currentFuel = 0.0f;
}


void UHandcarNitroFuelWidget::UpdateFuelProgressBarPercent()
{
	if (fuelProgressBar)
	{
		const float _fuelPercent = currentFuel / maxFuel;
		fuelProgressBar->SetPercent(_fuelPercent);
	}
}


void UHandcarNitroFuelWidget::SetMaxFuel(const float _maxFuel)
{
	maxFuel = _maxFuel; 
	
	if (maxFuelTextBlock)
	{
		const FText _maxFuelFText = CreateFTextFromFloat(maxFuel);
		maxFuelTextBlock->SetText(_maxFuelFText);
	}

	const bool _updateCurrentFuel = currentFuel > maxFuel;

	if (_updateCurrentFuel)
		SetCurrentFuel(maxFuel);
	else
		UpdateFuelProgressBarPercent();
}

void UHandcarNitroFuelWidget::SetCurrentFuel(const float _currentFuel)
{
	currentFuel = _currentFuel;

	if (currentFuelTextBlock)
	{
		const FText _currentFuelFText = CreateFTextFromFloat(currentFuel);
		currentFuelTextBlock->SetText(_currentFuelFText);
	}

	UpdateFuelProgressBarPercent();
}
