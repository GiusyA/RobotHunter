#include "CameraInteractableActor.h"
#include "RobotHunter/Player/CustomPlayer.h"

ACameraInteractableActor::ACameraInteractableActor()
{
	movePlayer = true;
	isSimpleInteractable = false;

	customCameraComponent = CREATE_DEFAULT_SUBOBJECT(UCustomCameraComponent, "CameraComponent");
	SETUP_ATTACHMENT(customCameraComponent, RootComponent);
	AddOwnedComponent(customCameraComponent);

	if (customCameraComponent)
	{
		USpringArmComponent* _springArmComponent = customCameraComponent->GetSpringArmComponent();

		if (_springArmComponent)
			_springArmComponent->bDoCollisionTest = false;
	}
}


void ACameraInteractableActor::FirstInteraction(ACustomPlayer* _player, USceneComponent* _playerPosition)
{
	if (_player &&  customCameraComponent)
	{
		UCustomCameraComponent* _playerCameraComponent = _player->GetCustomCameraComponent();

		if (_playerCameraComponent)
		{
			const ECameraKey _playerCameraSettingsKey = _playerCameraComponent->GetCurrentSettingsKey();
			_player->SetCameraViewWithBlend(this, customCameraComponent->GetTransitionDuration(_playerCameraSettingsKey));
		}

		Super::FirstInteraction(_player, _playerPosition);
	}
}

void ACameraInteractableActor::SecondInteraction(ACustomPlayer* _player)
{
	if (_player && customCameraComponent)
	{
		UCustomCameraComponent* _playerCameraComponent = _player->GetCustomCameraComponent();

		if (_playerCameraComponent)
		{
			const ECameraKey _cameraSettingsKey = customCameraComponent->GetCurrentSettingsKey();
			_player->SetCameraViewWithBlend(_player, _playerCameraComponent->GetTransitionDuration(_cameraSettingsKey));
		}

		Super::SecondInteraction(_player);
	}
}
