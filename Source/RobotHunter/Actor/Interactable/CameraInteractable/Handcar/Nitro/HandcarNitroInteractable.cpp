#include "HandcarNitroInteractable.h"
#include "EnhancedInputComponent.h"
#include "RobotHunter/Player/CustomPlayer.h"
#include "RobotHunter/UI/HUD/CustomHUD.h"

AHandcarNitroInteractable::AHandcarNitroInteractable()
{
#pragma region Debug
	useAccelerationDebug = false;
	useFuelConsumptionDebug = false;
#pragma endregion


	valveMesh = CREATE_DEFAULT_SUBOBJECT(UStaticMeshComponent, "ValveMesh");
	SETUP_ATTACHMENT(valveMesh, RootComponent);

	acceleration = 100.0f;


#pragma region FuelConsumption
	greenZoneFuelConsumption = 5.0f;
	yellowZoneFuelConsumption = 10.0f;
	redZoneFuelConsumption = 15.0f;

	currentFuelConsumption = 0.0f;
#pragma endregion


	propertiesDA = nullptr;
	widget = nullptr;


	isActive = false;
	currentAcceleration = 0.0f;

	negateAcceleration = false;
}


void AHandcarNitroInteractable::InitializeWidget()
{
	if (!widget)
		widget = FindHandcarNitroWidget();

	if (widget)
	{
		widget->UpdatePropertiesFromDA(propertiesDA);
		widget->InitializeZones();
		widget->StartZonesMovement();

		if (valveMesh)
			valveMesh->SetWorldRotation(FRotator(0.0f, 0.0f, widget->GetCursorAngularOffset()));
	}
	else
	{
		const FTimerDelegate _delegate = FTimerDelegate::CreateLambda([&]() { InitializeWidget(); });
		FTimerHandle _initializeWidgetTimer = FTimerHandle();
		TIMER_MANAGER.SetTimer(_initializeWidgetTimer, _delegate, 0.1f, false);
	}
}

void AHandcarNitroInteractable::MoveWidgetCursor(const FInputActionValue& _value)
{
	if (widget)
	{
		const float _inputValue = _value.Get<float>();
		widget->MoveCursor(_inputValue);

		if (_inputValue != 0.0f)
			if (valveMesh)
				valveMesh->SetWorldRotation(FRotator(0.0f, 0.0f, widget->GetCursorAngularOffset()));
	}
}


void AHandcarNitroInteractable::UpdatePropertiesFromDA()
{
	if (propertiesDA)
	{
		propertiesDA->UpdateUseDebug(useRealtime, useDebug, useDebugTool);
		propertiesDA->UpdateUseNitroDebug(useAccelerationDebug, useFuelConsumptionDebug);

		propertiesDA->UpdateNitroAcceleration(acceleration);
		propertiesDA->UpdateFuelConsumption(greenZoneFuelConsumption, yellowZoneFuelConsumption, redZoneFuelConsumption);
	}
}

void AHandcarNitroInteractable::UpdateCurrentFuelConsumption()
{
	if (widget)
	{
		const ENitroZoneColor _cursorColor = widget->UpdateCursorColor(isActive);

		switch (_cursorColor)
		{
		case ENitroZoneColor::GreenZone:
		{
			currentFuelConsumption = greenZoneFuelConsumption;
			break;
		}

		case ENitroZoneColor::YellowZone:
		{
			currentFuelConsumption = yellowZoneFuelConsumption;
			break;
		}

		case ENitroZoneColor::RedZone:
		{
			currentFuelConsumption = redZoneFuelConsumption;
			break;
		}

		case ENitroZoneColor::BlackZone:
		{
			currentFuelConsumption = 0.0f;
			break;
		}

		default:
			break;
		}
	}
}

void AHandcarNitroInteractable::UpdateCurrentAcceleration()
{
	currentAcceleration = !isActive ? 0.0f
							: negateAcceleration ? -acceleration
							: acceleration;
}

UHandcarNitroWidget* AHandcarNitroInteractable::FindHandcarNitroWidget() const
{
	ACustomHUD* _hud = nullptr;
	APlayerController* _controller = FIRST_PLAYER_CONTROLLER;

	if (_controller)
		_hud = CAST(ACustomHUD, _controller->GetHUD());

	if (_hud)
		return CAST(UHandcarNitroWidget, _hud->GetWidget(EWidgetType::HandcarNitroWidget));

	return nullptr;
}


void AHandcarNitroInteractable::PrintDebug() const
{
	if (useAccelerationDebug)
		PRINT_SCREEN_WITH_FLOAT_TICK("[Handcar][Nitro] Acceleration : ", currentAcceleration, FColor::Green);

	if (useFuelConsumptionDebug)
		PRINT_SCREEN_WITH_FLOAT_TICK("[Handcar][Nitro] Fuel consumption : ", currentFuelConsumption, FColor::Red);
}


void AHandcarNitroInteractable::SetupPlayerInputs(ACustomPlayer* _player)
{
	if (_player)
	{
		UEnhancedInputComponent* _input = Cast<UEnhancedInputComponent>(_player->InputComponent);
		UInputConfigDA* _inputConfig = _player->GetInputConfig();

		if (_input && _inputConfig)
		{
			_input->BindAction(_inputConfig->GetHandcarNitroCursorAction(), ETriggerEvent::Triggered, this, &AHandcarNitroInteractable::MoveWidgetCursor);
			Super::SetupPlayerInputs(_player);
		}
	}
}


void AHandcarNitroInteractable::FirstInteraction(ACustomPlayer* _player, USceneComponent* _playerPosition)
{
	Super::FirstInteraction(_player, _playerPosition);

	if (_player)
		_player->SetFSMBool(EPlayerBool::HandcarNitroBool, true);
}

void AHandcarNitroInteractable::SecondInteraction(ACustomPlayer* _player)
{
	Super::SecondInteraction(_player);

	if (_player)
		_player->SetFSMBool(EPlayerBool::HandcarNitroBool, false);
}


void AHandcarNitroInteractable::NitroBeginPlay(UHandcarPropertiesDA* _da)
{
	propertiesDA = _da;
	UpdatePropertiesFromDA();
	InitializeWidget();
}

void AHandcarNitroInteractable::NitroTick(const float _deltaTime)
{
	if (isRuntime)
	{
		if (widget)
			widget->MoveZones(_deltaTime);

		UpdateCurrentFuelConsumption();
		UpdateCurrentAcceleration();
	}
}
