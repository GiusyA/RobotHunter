#include "HandcarPropertiesDA.h"

UHandcarPropertiesDA::UHandcarPropertiesDA()
{
#pragma region Debug
	useRealtime = false;
	useDebug = false;
	useDebugTool = false;


#pragma region Handcar
	useSpeedDebug = false;
	useFrictionDebug = false;
	useSlopeAccelerationDebug = false;
	useTotalAccelerationDebug = false;


#pragma region Nitro
	useNitroAccelerationDebug = false;
	useFuelConsumptionDebug = false;
#pragma endregion


#pragma region Handle
	useHandleAccelerationDebug = false;
	useFirstHeightRapportDebug = false;
	useSecondHeightRapportDebug = false;
#pragma endregion


#pragma endregion


#pragma endregion


#pragma region Handcar


#pragma region Movement


#pragma region Speed
	maxSpeed = 500.0f;
	rotationSpeed = 25.0f;
#pragma endregion


#pragma region Acceleration


#pragma region Friction
	friction = -3.0f;
	frictionSpeedPercent = 0.1f;
#pragma endregion


#pragma endregion


#pragma endregion


#pragma region Nitro
	nitroAcceleration = 100.0f;

#pragma region UI
	cursorSpeed = 5.0f;


#pragma region Zone


#pragma region Size
	greenZoneSize = 10.0f;
	yellowZoneSize = 40.0f;
#pragma endregion


#pragma region Movement


#pragma region Angle
	minMovementAngle = -50.0f;
	maxMovementAngle = 50.0f;
#pragma endregion


#pragma region Speed
	minMovementSpeed = 5.0f;
	maxMovementSpeed = 10.0f;
#pragma endregion


#pragma region Time
	minMovementTime = 2.0f;
	maxMovementTime = 5.0f;
#pragma endregion


#pragma endregion


#pragma endregion


#pragma endregion


#pragma region FuelConsumption
	greenZoneFuelConsumption = 5.0f;
	yellowZoneFuelConsumption = 10.0f;
	redZoneFuelConsumption = 15.0f;
#pragma endregion


#pragma endregion


#pragma region Handle


#pragma region Rotation
	handleMaxRotation = 45.0f;
	handleRotationValue = 50.0f;
#pragma endregion


#pragma region Acceleration
	handleMinForce = 5000.0f;
	handleMaxForce = 10000.0f;

	handleAccelerationBoost = 1.5f;
#pragma endregion

#pragma endregion


#pragma endregion
}


#pragma region Debug
void UHandcarPropertiesDA::UpdateUseDebug(bool& _useRealtime, bool& _useDebug, bool& _useDebugTool)
{
	_useRealtime = useRealtime;
	_useDebug = useDebug;
	_useDebugTool = useDebugTool;
}

void UHandcarPropertiesDA::UpdateUseHandcarDebug(bool& _useSpeedDebug, bool& _useFrictionDebug, bool& _useSlopeAccelerationDebug, bool& _useTotalAccelerationDebug)
{
	_useSpeedDebug = useSpeedDebug;
	_useFrictionDebug = useFrictionDebug;
	_useSlopeAccelerationDebug = useSlopeAccelerationDebug;
	_useTotalAccelerationDebug = useTotalAccelerationDebug;

}

void UHandcarPropertiesDA::UpdateUseNitroDebug(bool& _useAccelerationDebug, bool& _useFuelConsumptionDebug)
{
	_useAccelerationDebug = useNitroAccelerationDebug;
	_useFuelConsumptionDebug = useFuelConsumptionDebug;
}

void UHandcarPropertiesDA::UpdateUseHandleDebug(bool& _useAccelerationDebug, bool& _useFirstHeightRapportDebug, bool& _useSecondHeightRapportDebug)
{
	_useAccelerationDebug = useHandleAccelerationDebug;
	_useFirstHeightRapportDebug = useFirstHeightRapportDebug;
	_useSecondHeightRapportDebug = useSecondHeightRapportDebug;

}
#pragma endregion


#pragma region Handcar


#pragma region Movement
void UHandcarPropertiesDA::UpdateSpeed(float& _maxSpeed, float& _rotationSpeed)
{
	_maxSpeed = maxSpeed;
	_rotationSpeed = rotationSpeed;
}

void UHandcarPropertiesDA::UpdateFriction(float& _friction, float& _frictionSpeedPercent)
{
	_friction = friction;
	_frictionSpeedPercent = frictionSpeedPercent;
}
#pragma endregion


#pragma region Nitro


#pragma region UI
void UHandcarPropertiesDA::UpdateCursorSpeed(float& _cursorSpeed)
{
	_cursorSpeed = cursorSpeed;
}


void UHandcarPropertiesDA::UpdateZoneSize(float& _greenZoneSize, float& _yellowZoneSize)
{
	_greenZoneSize = greenZoneSize;
	_yellowZoneSize = yellowZoneSize;
}


#pragma region Movement
void UHandcarPropertiesDA::UpdateMovementAngle(float& _minMovementAngle, float& _maxMovementAngle)
{
	_minMovementAngle = minMovementAngle;
	_maxMovementAngle = maxMovementAngle;
}

void UHandcarPropertiesDA::UpdateMovementSpeed(float& _minMovementSpeed, float& _maxMovementSpeed)
{
	_minMovementSpeed = minMovementSpeed;
	_maxMovementSpeed = maxMovementSpeed;
}

void UHandcarPropertiesDA::UpdateMovementTime(float& _minMovementTime, float& _maxMovementTime)
{
	_minMovementTime = minMovementTime;
	_maxMovementTime = maxMovementTime;
}
#pragma endregion


#pragma endregion

void UHandcarPropertiesDA::UpdateNitroAcceleration(float& _acceleration)
{
	_acceleration = nitroAcceleration;
}

void UHandcarPropertiesDA::UpdateFuelConsumption(float& _greenZoneFuelConsumption, float& _yellowZoneFuelConsumption, float& _redZoneFuelConsumption)
{
	_greenZoneFuelConsumption = greenZoneFuelConsumption;
	_yellowZoneFuelConsumption = yellowZoneFuelConsumption;
	_redZoneFuelConsumption = redZoneFuelConsumption;
}
#pragma endregion


#pragma region Handle
void UHandcarPropertiesDA::UpdateHandleMeshRotation(float& _maxRotation, float& _rotationValue)
{
	_maxRotation = handleMaxRotation;
	_rotationValue = handleRotationValue;
}

void UHandcarPropertiesDA::UpdateHandleForce(float& _minForce, float& _maxForce)
{
	_minForce = handleMinForce;
	_maxForce = handleMaxForce;
}

void UHandcarPropertiesDA::UpdateHandleAccelerationBoost(float& _boost)
{
	_boost = handleAccelerationBoost;
}
#pragma endregion


#pragma endregion