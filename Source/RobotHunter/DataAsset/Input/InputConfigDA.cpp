#include "InputConfigDA.h"

UInputConfigDA::UInputConfigDA()
{
#pragma region Context
	gameContext = nullptr;
#pragma endregion


#pragma region Action
	moveAction = nullptr;
	rotateCameraAction = nullptr;
	jumpAction = nullptr;

	interactionAction = nullptr;

	aimAction = nullptr;
#pragma endregion


#pragma region Handcar
	handcarNitroCursorAction = nullptr;


#pragma region Brake
	handcarBrakeTightenAction = nullptr;
	handcarBrakeReleaseAction = nullptr;
#pragma endregion


#pragma region Handle
	handcarHandleAcceleration = nullptr;
	handcarHandleChangeWay = nullptr;
#pragma endregion


#pragma endregion
}
