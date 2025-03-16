#include "CustomPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ACustomPlayer::ACustomPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	useRealtime = false;


#pragma region Input
	inputConfig = nullptr;
	currentInputContext = nullptr;

	inputEnabled = true;
#pragma endregion


#pragma region Component


#pragma region Camera
	customCameraComponent = CREATE_DEFAULT_SUBOBJECT(UCustomCameraComponent, "CameraComponent");
	SETUP_ATTACHMENT(customCameraComponent, RootComponent);
	AddOwnedComponent(customCameraComponent);

	endBlendCameraDelegate = FTimerDelegate();
#pragma endregion


	interactionComponent = CREATE_DEFAULT_SUBOBJECT(UInteractionComponent, "InteractionComponent");
	SETUP_ATTACHMENT(interactionComponent, RootComponent);
	AddOwnedComponent(interactionComponent);

	fsmComponent = CREATE_DEFAULT_SUBOBJECT(UPlayerFSMComponent, "FSMComponent");
	SETUP_ATTACHMENT(fsmComponent, RootComponent);
	AddOwnedComponent(fsmComponent);
#pragma endregion


	playerMesh = nullptr;
	meshBaseYaw = 0.0f;
}


void ACustomPlayer::SetMeshRotation(const FRotator& _rotation)
{
	if (playerMesh)
	{
		const FRotator _newRotation = FRotator(0.0f, meshBaseYaw, 0.0f) + _rotation;
		playerMesh->SetWorldRotation(_newRotation);
		playerMesh->SetRelativeRotation(_newRotation);
	}
}


void ACustomPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (inputConfig)
		EnableInputContext(inputConfig->GetGameContext());

	if (interactionComponent)
		interactionComponent->SetPlayer(this);

	if (fsmComponent)
		fsmComponent->Start();

	playerMesh = GetMesh();

	if (playerMesh)
		meshBaseYaw = playerMesh->GetRelativeRotation().Yaw;
}

void ACustomPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (inputConfig)
	{
		UEnhancedInputComponent* _inputComp = CAST(UEnhancedInputComponent, PlayerInputComponent);

		if (_inputComp)
		{
			_inputComp->BindAction(inputConfig->GetMoveAction(), ETriggerEvent::Triggered, this, &ACustomPlayer::MoveTriggered);
			_inputComp->BindAction(inputConfig->GetRotateCameraAction(), ETriggerEvent::Triggered, this, &ACustomPlayer::RotateCameraTriggered);
			_inputComp->BindAction(inputConfig->GetJumpAction(), ETriggerEvent::Started, this, &ACustomPlayer::JumpStarted);

			_inputComp->BindAction(inputConfig->GetInteractionAction(), ETriggerEvent::Started, this, &ACustomPlayer::InteractStarted);
			_inputComp->BindAction(inputConfig->GetInteractionAction(), ETriggerEvent::Completed, this, &ACustomPlayer::InteractCompleted);
		}
	}
}

bool ACustomPlayer::ShouldTickIfViewportsOnly() const
{
	return useRealtime;
}


#pragma region Input
void ACustomPlayer::MoveTriggered(const FInputActionValue& _value)
{
	if (inputEnabled && customCameraComponent)
	{
		if (playerMesh)
		{
			const FRotator _cameraRot = customCameraComponent->GetRelativeRotation();

			const FRotator _meshRot = playerMesh->GetComponentRotation();
			const FRotator _newMeshRot = FRotator(_meshRot.Pitch, _cameraRot.Yaw, _meshRot.Roll);
			SetMeshRotation(_newMeshRot);
		}

		const FVector2D _inputValue = _value.Get<FVector2D>();
		const FVector _movement = customCameraComponent->GetForwardVector() * _inputValue.X + customCameraComponent->GetRightVector().GetSafeNormal() * _inputValue.Y;
		AddMovementInput(_movement);
		onMove.Broadcast(this, _movement);
	}
}

void ACustomPlayer::RotateCameraTriggered(const FInputActionValue& _value)
{
	if (inputEnabled && customCameraComponent)
	{
		const FVector2D _inputValue = _value.Get<FVector2D>();
		FRotator _rotation = FRotator(_inputValue.Y, _inputValue.X, 0.0f);
		customCameraComponent->AddRelativeRotation(_rotation);
	}
}

void ACustomPlayer::JumpStarted(const FInputActionValue& _value)
{
	if (inputEnabled)
		Jump();
}


void ACustomPlayer::InteractStarted(const FInputActionValue& _value)
{
	if (inputEnabled && interactionComponent)
		interactionComponent->InteractStarted();
}

void ACustomPlayer::InteractCompleted(const FInputActionValue& _value)
{
	if (inputEnabled && interactionComponent)
		interactionComponent->InteractCompleted();
}


void ACustomPlayer::EnableInputContext(UInputMappingContext* _context)
{
	if (_context)
	{
		APlayerController* _controller = FIRST_PLAYER_CONTROLLER;

		if (_controller)
		{
			UEnhancedInputLocalPlayerSubsystem* _inputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(_controller->GetLocalPlayer());

			if (_inputSubsystem)
			{
				_inputSubsystem->ClearAllMappings();
				_inputSubsystem->AddMappingContext(_context, 0);
				currentInputContext = _context;
			}
		}
	}
}

void ACustomPlayer::DisableCurrentInputContext()
{
	if (currentInputContext)
	{
		APlayerController* _controller = FIRST_PLAYER_CONTROLLER;

		if (_controller)
		{
			UEnhancedInputLocalPlayerSubsystem* _inputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(_controller->GetLocalPlayer());

			if (_inputSubsystem)
			{
				_inputSubsystem->RemoveMappingContext(currentInputContext);
				currentInputContext = nullptr;
			}
		}
	}
}
#pragma endregion


void ACustomPlayer::SetActorRotation(const FRotator& _rotation, const bool _updateCamera)
{
	if (customCameraComponent && _updateCamera)
	{
		FRotator _newCameraRot = FRotator(0.0f, _rotation.Yaw, 0.0f);
		customCameraComponent->SetRelativeRotation(_newCameraRot);
	}

	SetMeshRotation(_rotation);
}


void ACustomPlayer::UpdateCameraCurrentSettings(const FString& _key)
{
	if (customCameraComponent)
		customCameraComponent->UpdateCurrentSettings(_key);
}

void ACustomPlayer::SetCameraViewWithBlend(AActor* _newView, const float _blendSpeed)
{
	SetInputEnabled(false);

	const FTimerDelegate _delegate = FTimerDelegate::CreateLambda([&]()
		{
			SetInputEnabled(true);
		});

	if (_newView)
	{
		APlayerController* _controller = FIRST_PLAYER_CONTROLLER;

		if (_controller)
		{
			FTimerHandle _blendTimer = FTimerHandle();
			_controller->SetViewTargetWithBlend(_newView, _blendSpeed);
			TIMER_MANAGER.SetTimer(_blendTimer, _delegate, _blendSpeed, false);
		}
	}
}

