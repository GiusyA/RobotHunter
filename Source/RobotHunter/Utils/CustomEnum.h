#pragma once


#pragma region Widget
UENUM()
enum EWidgetType
{
	HandcarNitroWidget,

	DefaultWidgetType
};
#pragma endregion


#pragma region FSM
UENUM()
enum EPlayerBool
{
	HandcarNitroBool,
	HandcarHandleBool,

	DefaultPlayerBool
};
#pragma endregion


#pragma region Rail
UENUM()
enum ERailWay
{
	DepartureWay,
	ReturnWay,

	DefaultWay
};
#pragma endregion

#pragma region Handcar

#pragma region Nitro
UENUM()
enum ENitroZoneColor
{
	GreenZone,
	YellowZone,
	RedZone,
	BlackZone,

	DefaultZone
};
#pragma endregion

#pragma endregion