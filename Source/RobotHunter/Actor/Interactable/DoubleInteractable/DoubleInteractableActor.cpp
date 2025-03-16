#include "DoubleInteractableActor.h"
#include "RobotHunter/Player/CustomPlayer.h"

ADoubleInteractableActor::ADoubleInteractableActor()
{
	movePlayer = true;
	isSimpleInteractable = false;


	secondInteractionText = CREATE_DEFAULT_SUBOBJECT(UTextRenderComponent, "SecondInteractionText");
	SETUP_ATTACHMENT(secondInteractionText, RootComponent);

	if (secondInteractionText)
	{
		secondInteractionText->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
		secondInteractionText->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	}


	secondPlayerPosition = CREATE_DEFAULT_SUBOBJECT(USceneComponent, "SecondPlayerPosition");
	SETUP_ATTACHMENT(secondPlayerPosition, RootComponent);

	secondPlayer = nullptr;


	secondSideDetected = false;
}


void ADoubleInteractableActor::DetectInteractable_Internal(ACustomPlayer* _player, const bool _isDetected)
{
	if (_player)
	{
		USceneComponent* _closestPosition = FindClosestPosition(_player);

		const bool _firstSideDetected = _closestPosition == playerPosition ? _isDetected
											: false;

		const bool _secondSideDetected = _closestPosition == secondPlayerPosition ? _isDetected
											: false;

		Super::DetectInteractable(_player, _firstSideDetected);

		if (secondSideDetected && secondPlayer == _player || !secondSideDetected && secondPlayer == nullptr)
		{
			secondSideDetected = _secondSideDetected;

			if (secondInteractionText)
				secondInteractionText->SetVisibility(_secondSideDetected);

			secondPlayer = secondSideDetected ? _player : nullptr;
		}
	}
}


void ADoubleInteractableActor::UpdateBindOnPlayerMove(ACustomPlayer* _player)
{
	if (_player)
	{
		player == _player || secondPlayer == _player ? _player->GetOnMove().AddUniqueDynamic(this, &ADoubleInteractableActor::UpdateSideDetected)
			: _player->GetOnMove().RemoveDynamic(this, &ADoubleInteractableActor::UpdateSideDetected);
	}
}

void ADoubleInteractableActor::UpdateSideDetected(ACustomPlayer* _player, const FVector& _movement)
{
	DetectInteractable_Internal(_player, true);
}


USceneComponent* ADoubleInteractableActor::FindClosestPosition(ACustomPlayer* _player) const
{
	if (_player)
	{
		const FVector _playerLoc = _player->GetActorLocation();
		const FVector _firstPosition = playerPosition->GetComponentLocation();
		const FVector _secondPosition = secondPlayerPosition->GetComponentLocation();

		const float _firstPositionDistance = FVector::Distance(_playerLoc, _firstPosition);
		const float _secondPositionDistance = FVector::Distance(_playerLoc, _secondPosition);

		return FMath::Abs(_secondPositionDistance) > FMath::Abs(_firstPositionDistance) ? playerPosition
				: secondPlayerPosition;
	}

	return nullptr;
}


void ADoubleInteractableActor::BeginPlay()
{
	Super::BeginPlay();

	if (secondInteractionText)
		secondInteractionText->SetVisibility(false);
}

void ADoubleInteractableActor::TickRotateInteractionText()
{
	Super::TickRotateInteractionText();

	if (secondSideDetected)
		RotateInteractionText(secondInteractionText);
}


void ADoubleInteractableActor::InteractionStarted(ACustomPlayer* _player, bool& _inInteraction)
{
	if (player == _player)
		InteractionStarted_Internal(player, playerPosition, _inInteraction);

	if (secondPlayer == _player)
		InteractionStarted_Internal(secondPlayer, secondPlayerPosition, _inInteraction);
}

void ADoubleInteractableActor::InteractionCompleted(ACustomPlayer* _player, bool& _inInteraction)
{
	if (player == _player)
		InteractionCompleted_Internal(player, _inInteraction);

	if (secondPlayer == _player)
		InteractionCompleted_Internal(secondPlayer, _inInteraction);
}


void ADoubleInteractableActor::DetectInteractable(ACustomPlayer* _player, const bool _isDetected)
{
	DetectInteractable_Internal(_player, _isDetected);
	UpdateBindOnPlayerMove(_player);
}
