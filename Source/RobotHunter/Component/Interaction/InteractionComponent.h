#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Component/CustomActorComponent.h"
#include "Components/SphereComponent.h"
#include "RobotHunter/Actor/Interactable/InteractableActor.h"
#include "InteractionComponent.generated.h"

UCLASS()
class ROBOTHUNTER_API UInteractionComponent : public UCustomActorComponent
{
	GENERATED_BODY()


#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Custom Property")
	TObjectPtr<USphereComponent> interactionSphere;


	UPROPERTY(VisibleAnywhere, Category = "Custom Property")
	TMap<FString, AInteractableActor*> interactablesInRange;

	UPROPERTY()
	TObjectPtr<AInteractableActor> currentInteractable;


	UPROPERTY()
	TObjectPtr<ACustomPlayer> player;


	bool inInteraction;
#pragma endregion


#pragma region Setter/Getter
public:
	FORCEINLINE void SetPlayer(ACustomPlayer* _player) { player = _player; }

	FORCEINLINE bool GetInInteraction() const { return inInteraction; }
	FORCEINLINE AInteractableActor* GetCurrentInteractable() const { return currentInteractable; }
#pragma endregion


#pragma region Functions
public:
	UInteractionComponent();

private:
	UFUNCTION() void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void AddActor(const FString& _key, AInteractableActor* _actor);
	void RemoveActor(AInteractableActor* _actor);
	AInteractableActor* FindClosestInteractable() const;

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupAttachment(USceneComponent* _root) const override;
	void InteractStarted();
	void InteractCompleted();
#pragma endregion
};
