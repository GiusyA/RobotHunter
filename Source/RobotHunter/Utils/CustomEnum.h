#pragma once


#pragma region Lerp
UENUM()
enum ECustomLerpType
{
	DefaultLerpType,

	FloatLerpType,
	VectorLerpType,
	RotatorLerpType
};
#pragma endregion


#pragma region Widget
UENUM()
enum EWidgetType
{
	DefaultWidgetType,

	ExplorationWidget,
	AimWidget,

	HandcarNitroWidget,
	HandcarNitroFuelWidget,
	HandcarBrakeWidget,
	HandcarHandleWidget,
};
#pragma endregion


#pragma region Camera
UENUM()
enum ECameraKey
{
	DefaultCameraKey,

	ExplorationCameraKey,
	AimCameraKey
};
#pragma endregion


#pragma region FSM
UENUM()
enum EPlayerBool
{
	DefaultPlayerBool,

	AimBool,

	HandcarNitroBool,
	HandcarBrakeBool,
	HandcarHandleBool,

	CountPlayerBool UMETA(Hidden)
};
#pragma endregion


#pragma region Rail
UENUM()
enum ERailWay
{
	DefaultWay,

	DepartureWay,
	ReturnWay,
};
#pragma endregion

#pragma region Handcar

#pragma region Nitro
UENUM()
enum ENitroZoneColor
{
	DefaultZone,

	GreenZone,
	YellowZone,
	RedZone,
	BlackZone,
};
#pragma endregion

#pragma endregion