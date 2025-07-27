#include "HandcarNitroWidget.h"

UHandcarNitroWidget::UHandcarNitroWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	cursorSlider = nullptr;
	cursorSpeed = 5.0f;
	currentCursorAngle = 30.0f;


#pragma region Zone
	blackSlider = nullptr;
	redSlider = nullptr;
	yellowSlider = nullptr;
	greenSlider = nullptr;


#pragma region Size
	greenZoneSize = 10.0f;
	yellowZoneSize = 40.0f;
#pragma endregion


#pragma region Movement


#pragma region Angle
	minMovementAngle = -50.0f;
	maxMovementAngle = 50.0f;

	greenInitialAngularOffset = 0.0f;
	yellowInitialAngularOffset = 0.0f;

	zoneTargetAngle = 0.0f;
	currentZoneAngle = 0.0f;
#pragma endregion


#pragma region Speed
	minMovementSpeed = 5.0f;
	maxMovementSpeed = 10.0f;

	currentZoneSpeed = 0.0f;
#pragma endregion


#pragma region Time
	minMovementTime = 2.0f;
	maxMovementTime = 5.0f;

	movementTimer = FTimerHandle();
#pragma endregion


#pragma endregion

	zonesAreMoving = false;
#pragma endregion
}


bool UHandcarNitroWidget::CheckIfCursorIsInSliderZone(const float _cursorValue, URadialSlider* _slider) const
{
	if (_slider)
	{
		const float _minValue = _slider->SliderHandleStartAngle;
		const float _maxValue = _slider->SliderHandleEndAngle;

		const float _zoneSize = _maxValue - _minValue;
		const float _zoneAngle = _slider->AngularOffset;

		const float _zoneStart = _minValue + _zoneAngle;
		const float _zoneEnd = _zoneStart + _zoneSize;

		if (_cursorValue >= _zoneStart && _cursorValue <= _zoneEnd)
			return true;
	}

	return false;
}

URadialSlider* UHandcarNitroWidget::FindSliderFromCursorValue(const float _cursorValue) const
{
	if (CheckIfCursorIsInSliderZone(_cursorValue, greenSlider))
		return greenSlider;

	if (CheckIfCursorIsInSliderZone(_cursorValue, yellowSlider))
		return yellowSlider;

	if (CheckIfCursorIsInSliderZone(_cursorValue, redSlider))
		return redSlider;

	return blackSlider;
}


void UHandcarNitroWidget::UpdatePropertiesFromDA(UHandcarPropertiesDA* _da)
{
	if (_da)
	{
		_da->UpdateNitroUICursorSpeed(cursorSpeed);

		_da->UpdateNitroUIZoneSize(greenZoneSize, yellowZoneSize);

		_da->UpdateNitroUIMovementAngle(minMovementAngle, maxMovementAngle);
		_da->UpdateNitroUIMovementSpeed(minMovementSpeed, maxMovementSpeed);
		_da->UpdateNitroUIMovementTime(minMovementTime, maxMovementTime);
	}
}


void UHandcarNitroWidget::InitializeZones()
{
	if (greenSlider && yellowSlider && blackSlider)
	{
		const float _minBlackSlider = blackSlider->SliderHandleStartAngle;
		const float _maxBlackSlider = blackSlider->SliderHandleEndAngle;
		const float _totalZoneSize = _maxBlackSlider - _minBlackSlider;


		float _greenZoneSize = FMath::CeilToFloat(_totalZoneSize * (greenZoneSize / 100.0f));
		float _yellowZoneSize = FMath::CeilToFloat(_totalZoneSize * (yellowZoneSize / 100.0f));

		CLAMP_ANGLE(_greenZoneSize);
		CLAMP_ANGLE(_yellowZoneSize);

		greenSlider->SetSliderHandleEndAngle(_greenZoneSize);
		yellowSlider->SetSliderHandleEndAngle(_yellowZoneSize);


		greenInitialAngularOffset = 180.0f - (_greenZoneSize / 2.0f);
		yellowInitialAngularOffset = 180.0f - (_yellowZoneSize / 2.0f);

		CLAMP_ANGLE(greenInitialAngularOffset);
		CLAMP_ANGLE(yellowInitialAngularOffset);

		greenSlider->SetAngularOffset(greenInitialAngularOffset);
		yellowSlider->SetAngularOffset(yellowInitialAngularOffset);
	}
}

void UHandcarNitroWidget::StartZonesMovement()
{
	FTimerManager& _tm = TIMER_MANAGER;

	if (_tm.IsTimerActive(movementTimer))
		_tm.ClearTimer(movementTimer);

	currentZoneSpeed = FMath::FRandRange(minMovementSpeed, maxMovementSpeed);
	zoneTargetAngle = FMath::FRandRange(minMovementAngle, maxMovementAngle);

	const float _movementTime = FMath::FRandRange(minMovementTime, maxMovementTime);
	const FTimerDelegate _delegate = FTimerDelegate::CreateLambda([&]() { zonesAreMoving = true; });
	_tm.SetTimer(movementTimer, _delegate, _movementTime, false);
}

void UHandcarNitroWidget::MoveZones(const float _deltaTime)
{
	if (greenSlider && yellowSlider)
	{
		if (zonesAreMoving)
		{
			currentZoneAngle += (zoneTargetAngle < currentZoneAngle) ? -currentZoneSpeed * _deltaTime
								: currentZoneSpeed * _deltaTime;

			if (FMath::IsNearlyEqual(zoneTargetAngle, currentZoneAngle, 0.1f))
			{
				currentZoneAngle = zoneTargetAngle;
				zonesAreMoving = false;

				StartZonesMovement();
			}

			greenSlider->SetAngularOffset(greenInitialAngularOffset + currentZoneAngle);
			yellowSlider->SetAngularOffset(yellowInitialAngularOffset + currentZoneAngle);
		}
	}
}


void UHandcarNitroWidget::MoveCursor(const float _inputValue)
{
	if (cursorSlider)
	{
		if (_inputValue != 0.0f)
		{
			//I divide the speed by 10 here because I find using a «integer» in the blueprint easier but I don't want a speed too high.
			float _newAngle = cursorSlider->AngularOffset + ((cursorSpeed / 10.0f) * _inputValue);
			
			if (blackSlider)
				CLAMP(_newAngle, blackSlider->SliderHandleStartAngle + 1.0f, blackSlider->SliderHandleEndAngle - 1.0f);

			currentCursorAngle = _newAngle;
		}

		cursorSlider->SetAngularOffset(currentCursorAngle);
	}
}

ENitroZoneColor UHandcarNitroWidget::UpdateCursorColor(bool& _nitroIsActive)
{
	if (cursorSlider)
	{
		const float _cursorValue = cursorSlider->AngularOffset;
		URadialSlider* _cursorZone = FindSliderFromCursorValue(_cursorValue);

		if (_cursorZone)
		{
			const FLinearColor _color = _cursorZone->SliderBarColor;
			SetCursorSliderColor(_color);

			const ENitroZoneColor _zoneColor = GetNitroZoneColorFromSlider(_cursorZone);
			const bool _activateNitro = _zoneColor != ENitroZoneColor::DefaultZone && _zoneColor != ENitroZoneColor::BlackZone;
			_nitroIsActive = _activateNitro;

			return _zoneColor;
		}
	}

	return ENitroZoneColor::BlackZone;
}
