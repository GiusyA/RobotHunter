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
#pragma endregion


#pragma region Handcar
	handcarNitroCursorAction = nullptr;
	handcarHandleAcceleration = nullptr;
#pragma endregion
}
