#include "CustomPlayerState.h"
#include "RobotHunter/Player/CustomPlayer.h"
#include "RobotHunter/FSM/CustomFSM.h"
#include "RobotHunter/UI/HUD/CustomHUD.h"

UCustomPlayerState::UCustomPlayerState()
{
	cameraSettingsKey = ECameraKey::DefaultCameraKey;

	widgetTypes = TArray<TEnumAsByte<EWidgetType>>();

	inputContext = nullptr;

	player = nullptr;
	hud = nullptr;
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

void UCustomPlayerState::RetrieveHUD()
{
	if (!hud)
	{
		APlayerController* _controller = FIRST_PLAYER_CONTROLLER;

		if (_controller)
			hud = CAST(ACustomHUD, _controller->GetHUD());
	}
}


void UCustomPlayerState::UpdateCameraSettings()
{
	if (player)
		player->UpdateCameraCurrentSettings(cameraSettingsKey);
}

void UCustomPlayerState::ShowWidgets()
{
	if (hud)
		hud->SetWidgetsVisibility(true, widgetTypes);
}

void UCustomPlayerState::HideWidgets()
{
	if (hud)
		hud->SetWidgetsVisibility(false, widgetTypes);
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
	RetrieveHUD();

	UpdateCameraSettings();
	ShowWidgets();
	EnablePlayerInputContext();
}

void UCustomPlayerState::Exit()
{
	HideWidgets();
	DisablePlayerInputContext();
}
