#include "InteractionComponent.h"
#include "RobotHunter/Player/CustomPlayer.h"

UInteractionComponent::UInteractionComponent()
{
	interactionSphere = CREATE_DEFAULT_SUBOBJECT(USphereComponent, "InteractionSphere");
	interactionSphere->SetGenerateOverlapEvents(true);

	interactablesInRange = TMap<FString, AInteractableActor*>();
	currentInteractable = nullptr;

	player = nullptr;

	inInteraction = false;
}


void UInteractionComponent::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->GetClass()->IsChildOf<AInteractableActor>())
	{
		AInteractableActor* _interactable = Cast<AInteractableActor>(OtherActor);

		if (_interactable)
		{
			if (_interactable->GetCanBeDetected())
				AddActor(_interactable->GetName(), _interactable);
		}
	}
}

void UInteractionComponent::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->GetClass()->IsChildOf<AInteractableActor>())
	{
		AInteractableActor* _interactable = Cast<AInteractableActor>(OtherActor);

		if (_interactable)
			RemoveActor(_interactable);
	}
}


void UInteractionComponent::AddActor(const FString& _key, AInteractableActor* _actor)
{
	if (!_actor || interactablesInRange.Contains(_key))
		return;

	_actor->DetectInteractable(player, true);
	interactablesInRange.Add(_key, _actor);
}

void UInteractionComponent::RemoveActor(AInteractableActor* _actor)
{
	if (_actor)
	{
		const FString _key = _actor->GetName();

		if (interactablesInRange.Contains(_key))
		{
			_actor->DetectInteractable(player, false);
			interactablesInRange.Remove(_key);
		}
	}
}

AInteractableActor* UInteractionComponent::FindClosestInteractable() const
{
	AInteractableActor* _closestInteractable = nullptr;
	AInteractableActor* _interactable = nullptr;

	const FVector _ownerLoc = GetOwner()->GetActorLocation();
	FVector _actorLoc = FVector();

	float _distance = -1.0f;
	float _shortestDistance = 0.0f;

	int _index = 0;

	for (TPair<FString, AInteractableActor*> _pair : interactablesInRange)
	{
		_interactable = _pair.Value;

		if (_interactable)
		{
			_actorLoc = _interactable->GetActorLocation();
			_distance = FVector::Distance(_ownerLoc, _actorLoc);

			if (_index == 0 || _shortestDistance > _distance)
			{
				_shortestDistance = _distance;
				_closestInteractable = _interactable;
				_index = 1;
			}
		}
	}

	return _closestInteractable;
}


void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (interactionSphere)
	{
		interactionSphere->OnComponentBeginOverlap.AddDynamic(this, &UInteractionComponent::OnComponentBeginOverlap);
		interactionSphere->OnComponentEndOverlap.AddDynamic(this, &UInteractionComponent::OnComponentEndOverlap);
	}
}


void UInteractionComponent::SetupAttachment(USceneComponent* _root) const
{
	SETUP_ATTACHMENT(interactionSphere, _root);
}

void UInteractionComponent::InteractStarted()
{
	if (player)
	{
		if (!inInteraction)
			currentInteractable = FindClosestInteractable();

		if (currentInteractable)
			currentInteractable->InteractionStarted(player, inInteraction);
	}
}

void UInteractionComponent::InteractCompleted()
{
	if (player)
	{
		if (currentInteractable)
			currentInteractable->InteractionCompleted(player, inInteraction);

		if (!inInteraction)
			currentInteractable = nullptr;
	}
}
