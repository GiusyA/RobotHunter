#include "HandcarNitroInteractable.h"
#include "EnhancedInputComponent.h"
#include "RobotHunter/Player/CustomPlayer.h"
#include "RobotHunter/UI/HUD/CustomHUD.h"
#include "RobotHunter/Actor/Handcar/HandcarActor.h"

AHandcarNitroInteractable::AHandcarNitroInteractable()
{
#pragma region Debug
	useAccelerationDebug = false;
	useFuelConsumptionDebug = false;
#pragma endregion


	valveMesh = CREATE_DEFAULT_SUBOBJECT(UStaticMeshComponent, "ValveMesh");
	SETUP_ATTACHMENT(valveMesh, RootComponent);

	acceleration = 100.0f;


#pragma region Fuel
	maxFuel = 500.0f;

	hasFuel = true;
	currentFuel = 0.0f;


#pragma region Consumption
	greenZoneFuelConsumption = 5.0f;
	yellowZoneFuelConsumption = 10.0f;
	redZoneFuelConsumption = 15.0f;

	currentFuelConsumption = 0.0f;
#pragma endregion


#pragma endregion


#pragma region UI
	widget = nullptr;
	fuelWidget = nullptr;
#pragma endregion


	propertiesDA = nullptr;
	handcar = nullptr;


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
			valveMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, widget->GetCursorAngularOffset()));
	}
	else
	{
		const FTimerDelegate _delegate = FTimerDelegate::CreateLambda([&]() { InitializeWidget(); });
		FTimerHandle _initializeWidgetTimer = FTimerHandle();
		TIMER_MANAGER.SetTimer(_initializeWidgetTimer, _delegate, 0.1f, false);
	}
}

void AHandcarNitroInteractable::InitializeFuelWidget()
{
	if (!fuelWidget)
		fuelWidget = FindHandcarNitroFuelWidget();

	if (fuelWidget)
	{
		fuelWidget->SetMaxFuel(maxFuel);
		fuelWidget->SetCurrentFuel(maxFuel);
	}
	else
	{
		const FTimerDelegate _delegate = FTimerDelegate::CreateLambda([&]() { InitializeFuelWidget(); });
		FTimerHandle _initializeFuelWidgetTimer = FTimerHandle();
		TIMER_MANAGER.SetTimer(_initializeFuelWidgetTimer, _delegate, 0.1f, false);
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
				valveMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, widget->GetCursorAngularOffset()));
	}
}


void AHandcarNitroInteractable::UpdatePropertiesFromDA()
{
	if (propertiesDA)
	{
		propertiesDA->UpdateUseDebug(useRealtime, useDebug, useDebugTool);
		propertiesDA->UpdateNitroUseDebug(useAccelerationDebug, useFuelConsumptionDebug);

		propertiesDA->UpdateNitroAcceleration(acceleration);
		propertiesDA->UpdateNitroFuel(maxFuel, greenZoneFuelConsumption, yellowZoneFuelConsumption, redZoneFuelConsumption);
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
	currentAcceleration = !isActive || !hasFuel ? 0.0f
							: negateAcceleration ? -acceleration
							: acceleration;
}


void AHandcarNitroInteractable::ConsumeFuel(const float _deltaTime)
{
	if (currentFuelConsumption > 0.0f && currentFuel > 0.0f)
	{
		const float _newCurrentFuel = currentFuel - (currentFuelConsumption * _deltaTime);
		currentFuel = _newCurrentFuel > 0.0f ? _newCurrentFuel
						: 0.0f;

		if (fuelWidget)
			fuelWidget->SetCurrentFuel(currentFuel);
	}

	hasFuel = currentFuel > 0.0f;
}


UHandcarNitroWidget* AHandcarNitroInteractable::FindHandcarNitroWidget() const
{
	if (hud)
		return CAST(UHandcarNitroWidget, hud->GetWidget(EWidgetType::HandcarNitroWidget));

	return nullptr;
}

UHandcarNitroFuelWidget* AHandcarNitroInteractable::FindHandcarNitroFuelWidget() const
{
	if (hud)
		return CAST(UHandcarNitroFuelWidget, hud->GetWidget(EWidgetType::HandcarNitroFuelWidget));

	return nullptr;
}


void AHandcarNitroInteractable::PrintDebug() const
{
	Super::PrintDebug();

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
		const UInputConfigDA* _inputConfig = _player->GetInputConfig();

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

	updatePlayerMeshRoll = true;
}

void AHandcarNitroInteractable::SecondInteraction(ACustomPlayer* _player)
{
	Super::SecondInteraction(_player);

	if (_player)
		_player->SetFSMBool(EPlayerBool::HandcarNitroBool, false);

	updatePlayerMeshRoll = false;
}


void AHandcarNitroInteractable::NitroBeginPlay(UHandcarPropertiesDA* _da, AHandcarActor* _handcar)
{
	propertiesDA = _da;
	UpdatePropertiesFromDA();
	handcar = _handcar;

	currentFuel = maxFuel;
	InitializeWidget();
	InitializeFuelWidget();
}

void AHandcarNitroInteractable::NitroTick(const float _deltaTime)
{
	if (isRuntime)
	{
		if (updatePlayerMeshRoll && handcar)
		{
			const FRotator _handcarRot = handcar->GetActorRotation();
			UpdatePlayerMeshRoll(player, playerPosition, -_handcarRot.Pitch);
		}

		if (widget)
			widget->MoveZones(_deltaTime);

		UpdateCurrentFuelConsumption();
		UpdateCurrentAcceleration();

		ConsumeFuel(_deltaTime);
	}
}
