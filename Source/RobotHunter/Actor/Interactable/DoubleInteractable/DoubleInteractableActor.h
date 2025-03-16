#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Actor/Interactable/InteractableActor.h"
#include "DoubleInteractableActor.generated.h"

UCLASS()
class ROBOTHUNTER_API ADoubleInteractableActor : public AInteractableActor
{
	GENERATED_BODY()


#pragma region Properties
protected:
	UPROPERTY(EditAnywhere, Category = "Custom Property")
	TObjectPtr<UTextRenderComponent> secondInteractionText;


	UPROPERTY(EditAnywhere, Category = "Custom Property|Player")
	TObjectPtr<USceneComponent> secondPlayerPosition;

	UPROPERTY()
	TObjectPtr<ACustomPlayer> secondPlayer;


	bool secondSideDetected;
#pragma endregion


#pragma region Functions
public:
	ADoubleInteractableActor();

private:
	void DetectInteractable_Internal(ACustomPlayer* _player, const bool _isDetected);

	void UpdateBindOnPlayerMove(ACustomPlayer* _player);
	UFUNCTION() void UpdateSideDetected(ACustomPlayer* _player, const FVector& _movement);

	USceneComponent* FindClosestPosition(ACustomPlayer* _player) const;

protected:
	virtual void BeginPlay() override;
	virtual void TickRotateInteractionText() override;

public:
	virtual void InteractionStarted(ACustomPlayer* _player, bool& _inInteraction) override;
	virtual void InteractionCompleted(ACustomPlayer* _player, bool& _inInteraction) override;

	virtual void DetectInteractable(ACustomPlayer* _player, const bool _isDetected) override;
#pragma endregion
};
