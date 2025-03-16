#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RobotHunter/Utils/CustomEnum.h"
#include "HandcarPropertiesDA.generated.h"

UCLASS()
class ROBOTHUNTER_API UHandcarPropertiesDA : public UDataAsset
{
	GENERATED_BODY()
	

#pragma region Properties


#pragma region Debug
	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug")
	bool useRealtime;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug")
	bool useDebug;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug")
	bool useDebugTool;


#pragma region Handcar
	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug|Handcar", meta = (EditCondition = "useDebug", EditConditionHides))
	bool useSpeedDebug;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug|Handcar", meta = (EditCondition = "useDebug", EditConditionHides))
	bool useFrictionDebug;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug|Handcar", meta = (EditCondition = "useDebug", EditConditionHides))
	bool useSlopeAccelerationDebug;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug|Handcar", meta = (EditCondition = "useDebug", EditConditionHides))
	bool useTotalAccelerationDebug;


#pragma region Nitro
	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug|Handcar|Nitro", meta = (EditCondition = "useDebug", EditConditionHides))
	bool useNitroAccelerationDebug;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug|Handcar|Nitro", meta = (EditCondition = "useDebug", EditConditionHides))
	bool useFuelConsumptionDebug;
#pragma endregion


#pragma region Handle
	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug|Handcar|Handle", meta = (EditCondition = "useDebug", EditConditionHides))
	bool useHandleAccelerationDebug;


	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug|Handcar|Handle", meta = (EditCondition = "useDebug", EditConditionHides))
	bool useFirstHeightRapportDebug;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug|Handcar|Handle", meta = (EditCondition = "useDebug", EditConditionHides))
	bool useSecondHeightRapportDebug;
#pragma endregion


#pragma endregion


#pragma endregion


#pragma region Handcar


#pragma region Movement


#pragma region Speed
	UPROPERTY(EditAnywhere, Category = "Custom Property|Movement|Speed", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float maxSpeed;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Movement|Speed", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float rotationSpeed;
#pragma endregion


#pragma region Acceleration


#pragma region Friction
	UPROPERTY(EditAnywhere, Category = "Custom Property|Movement|Acceleration|Friction", meta = (UIMax = -0.1f, ClampMax = -0.1f))
	float friction;

	/// <summary>
	/// Multiplier applied to the substracted current speed from the friction.
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Custom Property|Movement|Acceleration|Friction", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 1.0f, ClampMax = 1.0f))
	float frictionSpeedPercent;
#pragma endregion


#pragma endregion


#pragma endregion


#pragma region Nitro
	UPROPERTY(EditAnywhere, Category = "Custom Property|Nitro|Acceleration", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float nitroAcceleration;


#pragma region UI
	UPROPERTY(EditAnywhere, Category = "Custom Property|Nitro|UI", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float cursorSpeed;


#pragma region Zone


#pragma region Size
	UPROPERTY(EditAnywhere, Category = "Custom Property|Nitro|UI|Zone|Size", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 100.0f, ClampMax = 100.0f))
	float greenZoneSize;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Nitro|UI|Zone|Size", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 100.0f, ClampMax = 100.0f))
	float yellowZoneSize;
#pragma endregion


#pragma endregion


#pragma region Movement


#pragma region Angle
	UPROPERTY(EditAnywhere, Category = "Custom Property|Nitro|UI|Zone|Movement|Angle", meta = (UIMin = -90.0f, ClampMin = -90.0f, UIMax = 90.0f, ClampMax = 90.0f))
	float minMovementAngle;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Nitro|UI|Zone|Movement|Angle", meta = (UIMin = -90.0, ClampMin = -90.0f, UIMax = 90.0f, ClampMax = 90.0f))
	float maxMovementAngle;
#pragma endregion


#pragma region Speed
	UPROPERTY(EditAnywhere, Category = "Custom Property|Nitro|UI|Zone|Movement|Speed", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float minMovementSpeed;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Nitro|UI|Zone|Movement|Speed", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float maxMovementSpeed;
#pragma endregion


#pragma region Time
	UPROPERTY(EditAnywhere, Category = "Custom Property|Nitro|UI|Zone|Movement|Time", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float minMovementTime;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Nitro|UI|Zone|Movement|Time", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float maxMovementTime;
#pragma endregion


#pragma endregion


#pragma endregion


#pragma region FuelConsumption
	UPROPERTY(EditAnywhere, Category = "Custom Property|Nitro|Fuel Consumption", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float greenZoneFuelConsumption;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Nitro|Fuel Consumption", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float yellowZoneFuelConsumption;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Nitro|Fuel Consumption", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float redZoneFuelConsumption;
#pragma endregion


#pragma endregion


#pragma region Handle


#pragma region Rotation
	UPROPERTY(EditAnywhere, Category = "Custom Property|Handle|Mesh|Rotation", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 360.0f, ClampMax = 360.0f))
	float handleMaxRotation;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Handle|Mesh|Rotation", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float handleRotationValue;
#pragma endregion


#pragma region Acceleration
	UPROPERTY(EditAnywhere, Category = "Custom Property|Handle|Acceleration", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float handleMinForce;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Handle|Acceleration", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float handleMaxForce;


	UPROPERTY(EditAnywhere, Category = "Custom Property|Handle|Acceleration", meta = (UIMin = 0.0f, ClampMin = 0.0f))
	float handleAccelerationBoost;
#pragma endregion


#pragma endregion


#pragma endregion


#pragma endregion


#pragma region Functions
public:
	UHandcarPropertiesDA();


public:
#pragma region Debug
	void UpdateUseDebug(bool& _useRealtime, bool& _useDebug, bool& _useDebugTool);
	void UpdateUseHandcarDebug(bool& _useSpeedDebug, bool& _useFrictionDebug, bool& _useSlopeAccelerationDebug, bool& _useTotalAccelerationDebug);
	void UpdateUseNitroDebug(bool& _useAccelerationDebug, bool& _useFuelConsumptionDebug);
	void UpdateUseHandleDebug(bool& _useAccelerationDebug, bool& _useFirstHeightRapportDebug, bool& _useSecondHeightRapportDebug);
#pragma endregion


#pragma region Handcar

#pragma region Movement
	void UpdateSpeed(float& _maxSpeed, float& _rotationSpeed);

	void UpdateFriction(float& _friction, float& _frictionSpeedPercent);
#pragma endregion

#pragma region Nitro

#pragma region UI
	void UpdateCursorSpeed(float& _cursorSpeed);

	void UpdateZoneSize(float& _greenZoneSize, float& _yellowZoneSize);

#pragma region Movement
	void UpdateMovementAngle(float& _minMovementAngle, float& _maxMovementAngle);
	void UpdateMovementSpeed(float& _minMovementSpeed, float& _maxMovementSpeed);
	void UpdateMovementTime(float& _minMovementTime, float& _maxMovementTime);
#pragma endregion

#pragma endregion

	void UpdateNitroAcceleration(float& _acceleration);
	void UpdateFuelConsumption(float& _greenZoneFuelConsumption, float& _yellowZoneFuelConsumption, float& _redZoneFuelConsumption);
#pragma endregion

#pragma region Handle
	void UpdateHandleMeshRotation(float& _maxRotation, float& _rotationValue);

	void UpdateHandleForce(float& _minForce, float& _maxForce);
	void UpdateHandleAccelerationBoost(float& _boost);
#pragma endregion

#pragma endregion

#pragma endregion
};
