#include "CameraInteractableActor.h"
#include "RobotHunter/Player/CustomPlayer.h"

ACameraInteractableActor::ACameraInteractableActor()
{
	movePlayer = true;
	isSimpleInteractable = false;

	customCameraComponent = CREATE_DEFAULT_SUBOBJECT(UCustomCameraComponent, "CameraComponent");
	SETUP_ATTACHMENT(customCameraComponent, RootComponent);
	AddOwnedComponent(customCameraComponent);
}


void ACameraInteractableActor::FirstInteraction(ACustomPlayer* _player, USceneComponent* _playerPosition)
{
	if (customCameraComponent)
	{
		_player->SetCameraViewWithBlend(this, customCameraComponent->GetCurrentSettings().blendSpeed);
		Super::FirstInteraction(_player, _playerPosition);
	}
}

void ACameraInteractableActor::SecondInteraction(ACustomPlayer* _player)
{
	if (_player)
	{
		_player->SetCameraViewWithBlend(_player, customCameraComponent->GetCurrentSettings().blendSpeed);
		Super::SecondInteraction(_player);
	}
}
