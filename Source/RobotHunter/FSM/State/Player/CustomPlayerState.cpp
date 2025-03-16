#include "CustomPlayerState.h"
#include "RobotHunter/Player/CustomPlayer.h"
#include "RobotHunter/FSM/CustomFSM.h"
#include "RobotHunter/UI/HUD/CustomHUD.h"

UCustomPlayerState::UCustomPlayerState()
{
	cameraSettingsKey = FString();
	widgetType = EWidgetType::DefaultWidgetType;

	inputContext = nullptr;

	player = nullptr;
}


void UCustomPlayerState::RetrievePlayer()
{
	if (!player)
	{
		if (fsm)
		{
			ACustomPlayer* _player = CAST(ACustomPlayer, fsm->GetOwner());
			player = _player;
		}
	}
}


void UCustomPlayerState::UpdateCameraSettings()
{
	if (player)
		player->UpdateCameraCurrentSettings(cameraSettingsKey);
}

void UCustomPlayerState::UpdateWidget()
{
	APlayerController* _controller = FIRST_PLAYER_CONTROLLER;

	if (_controller)
	{
		ACustomHUD* _hud = CAST(ACustomHUD, _controller->GetHUD());

		if (_hud)
			_hud->SetCurrentWidget(widgetType);
	}
}


void UCustomPlayerState::DisablePlayerInputContext()
{
	if (player)
		player->DisableCurrentInputContext();
}

void UCustomPlayerState::EnablePlayerInputContext()
{
	if (player)
		player->EnableInputContext(inputContext);
}


void UCustomPlayerState::Enter()
{
	RetrievePlayer();

	UpdateCameraSettings();
	UpdateWidget();
	EnablePlayerInputContext();
}

void UCustomPlayerState::Exit()
{
	DisablePlayerInputContext();
}
