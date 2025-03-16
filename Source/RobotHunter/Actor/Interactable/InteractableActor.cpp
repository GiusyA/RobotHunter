#include "InteractableActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "RobotHunter/Player/CustomPlayer.h"

AInteractableActor::AInteractableActor()
{
	interactionText = CREATE_DEFAULT_SUBOBJECT(UTextRenderComponent, "InteractionText");
	SETUP_ATTACHMENT(interactionText, RootComponent);

	if (interactionText)
	{
		interactionText->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
		interactionText->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	}


	playerPosition = CREATE_DEFAULT_SUBOBJECT(USceneComponent, "PlayerPosition");
	SETUP_ATTACHMENT(playerPosition, RootComponent);

	player = nullptr;

	playerInputConfigured = TArray<ACustomPlayer*>();


	canBeDetected = true;
	isDetected = false;

	movePlayer = false;
	isSimpleInteractable = true;
}


void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();

	if (interactionText)
		interactionText->SetVisibility(false);
}

void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isRuntime)
		TickRotateInteractionText();
}


void AInteractableActor::InteractionStarted_Internal(ACustomPlayer* _player, USceneComponent* _playerPosition, bool& _inInteraction)
{
	if (_player)
	{
		if (!playerInputConfigured.Contains(_player))
			SetupPlayerInputs(_player);

		if (!isSimpleInteractable)
		{
			if (!_inInteraction)
				FirstInteraction(_player, _playerPosition);
			else
				SecondInteraction(_player);

			_inInteraction = !_inInteraction;
		}
		else
			_inInteraction = true;
	}
}

void AInteractableActor::InteractionCompleted_Internal(ACustomPlayer* _player, bool& _inInteraction)
{
	if (_player)
	{
		if (isSimpleInteractable)
			_inInteraction = false;
	}
}


void AInteractableActor::FirstInteraction(ACustomPlayer* _player, USceneComponent* _playerPosition)
{
	if (movePlayer)
	{
		if (_player && _playerPosition)
		{
			_player->SetActorLocation(_playerPosition->GetComponentLocation());
			_player->SetActorRotation(_playerPosition->GetRelativeRotation());
		}
	}
}

void AInteractableActor::SecondInteraction(ACustomPlayer* _player)
{
	if (movePlayer)
	{
		if (_player)
		{
			const FRotator _cameraRot = _player->GetCameraRelativeRotation();
			const FRotator _meshRot = _player->GetMeshRotation();
			_player->SetActorRotation(FRotator(_meshRot.Pitch, _cameraRot.Yaw, _meshRot.Roll));
		}
	}
}


void AInteractableActor::SetupPlayerInputs(ACustomPlayer* _player)
{
	if (_player)
		playerInputConfigured.Add(_player);
}

void AInteractableActor::TickRotateInteractionText()
{
	if (isDetected)
		RotateInteractionText(interactionText);
}


void AInteractableActor::RotateInteractionText(UTextRenderComponent* _textComp)
{
	if (_textComp)
	{
		FVector _cameraLoc = FVector();
		FVector _cameraDir = FVector();

		APlayerController* _controller = FIRST_PLAYER_CONTROLLER;

		if (_controller)
			UGameplayStatics::DeprojectScreenToWorld(_controller, FVector2D(0.5f, 0.5f), _cameraLoc, _cameraDir);

		FRotator _newRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), _cameraLoc);
		_textComp->SetRelativeRotation(_newRot);
	}
}


void AInteractableActor::InteractionStarted(ACustomPlayer* _player, bool& _inInteraction)
{
	InteractionStarted_Internal(player, playerPosition, _inInteraction);
}

void AInteractableActor::InteractionCompleted(ACustomPlayer* _player, bool& _inInteraction)
{
	InteractionCompleted_Internal(player, _inInteraction);
}


void AInteractableActor::DetectInteractable(ACustomPlayer* _player, const bool _isDetected)
{
	if (_player)
	{
		if (isDetected && player == _player || !isDetected && player == nullptr)
		{
			isDetected = _isDetected;

			if (interactionText)
				interactionText->SetVisibility(_isDetected);

			player = isDetected ? _player : nullptr;
		}
	}
}
