#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/UserWidget/CustomUserWidget.h"
#include "Components/RadialSlider.h"
#include "RobotHunter/DataAsset/Handcar/HandcarPropertiesDA.h"
#include "HandcarNitroWidget.generated.h"

UCLASS()
class ROBOTHUNTER_API UHandcarNitroWidget : public UCustomUserWidget
{
	GENERATED_BODY()


#pragma region Properties
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<URadialSlider> cursorSlider;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Cursor", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float cursorSpeed;

	float currentCursorAngle;


#pragma region Zone
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<URadialSlider> blackSlider;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<URadialSlider> redSlider;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<URadialSlider> yellowSlider;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<URadialSlider> greenSlider;


#pragma region Size
	UPROPERTY(EditAnywhere, Category = "Custom Property|Zone|Size", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 100.0f, ClampMax = 100.0f))
	float greenZoneSize;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Zone|Size", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 100.0f, ClampMax = 100.0f))
	float yellowZoneSize;
#pragma endregion


#pragma region Movement


#pragma region Angle
	UPROPERTY(EditAnywhere, Category = "Custom Property|Zone|Movement|Angle", meta = (UIMin = -90.0f, ClampMin = -90.0f, UIMax = 90.0f, ClampMax = 90.0f))
	float minMovementAngle;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Zone|Movement|Angle", meta = (UIMin = -90.0, ClampMin = -90.0f, UIMax = 90.0f, ClampMax = 90.0f))
	float maxMovementAngle;

	float greenInitialAngularOffset;
	float yellowInitialAngularOffset;

	float zoneTargetAngle;
	float currentZoneAngle;
#pragma endregion


#pragma region Speed
	UPROPERTY(EditAnywhere, Category = "Custom Property|Zone|Movement|Speed", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float minMovementSpeed;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Zone|Movement|Speed", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float maxMovementSpeed;

	float currentZoneSpeed;
#pragma endregion


#pragma region Time
	UPROPERTY(EditAnywhere, Category = "Custom Property|Zone|Movement|Time", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float minMovementTime;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Zone|Movement|Time", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float maxMovementTime;
#pragma endregion


	FTimerHandle movementTimer;
#pragma endregion


	bool zonesAreMoving;
#pragma endregion


#pragma endregion


#pragma region Functions
public:
	FORCEINLINE void SetCursorSliderColor(const FLinearColor& _color)
	{
		if (cursorSlider)
			cursorSlider->SetSliderBarColor(_color);
	}

	FORCEINLINE float GetCursorAngularOffset() const
	{
		return cursorSlider ? cursorSlider->AngularOffset
				: 0.0f;
	}

	FORCEINLINE ENitroZoneColor GetNitroZoneColorFromSlider(URadialSlider* _slider) const
	{
		ENitroZoneColor _color = ENitroZoneColor::BlackZone;

		if (_slider)
		{
			_color = _slider == greenSlider ? ENitroZoneColor::GreenZone
						: _slider == yellowSlider ? ENitroZoneColor::YellowZone
						: _slider == redSlider ? ENitroZoneColor::RedZone
						: _color;
		}

		return _color;
	}

public:
	UHandcarNitroWidget(const FObjectInitializer& ObjectInitializer);

private:
	bool CheckIfCursorIsInSliderZone(const float _cursorValue, URadialSlider* _slider) const;
	/// <summary>
	/// Find the slider that owns the zone in which the cursor is.
	/// </summary>
	URadialSlider* FindSliderFromCursorValue(const float _cursorValue) const;

public:
	void UpdatePropertiesFromDA(UHandcarPropertiesDA* _da);

	void InitializeZones();
	void StartZonesMovement();
	void MoveZones(const float _deltaTime);

	void MoveCursor(const float _inputValue);
	ENitroZoneColor UpdateCursorColor(bool& _nitroIsActive);
#pragma endregion
};
