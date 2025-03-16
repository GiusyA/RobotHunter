#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Actor/CustomActor.h"
#include "Components/TextRenderComponent.h"
#include "InteractableActor.generated.h"

class ACustomPlayer;

UCLASS()
class ROBOTHUNTER_API AInteractableActor : public ACustomActor
{
	GENERATED_BODY()


#pragma region Properties
protected:
	UPROPERTY(EditAnywhere, Category = "Custom Property")
	TObjectPtr<UTextRenderComponent> interactionText;


	UPROPERTY(EditAnywhere, Category = "Custom Property|Player")
	TObjectPtr<USceneComponent> playerPosition;

	UPROPERTY()
	TObjectPtr<ACustomPlayer> player;

	UPROPERTY()
	TArray<ACustomPlayer*> playerInputConfigured;


	bool canBeDetected;
	bool isDetected;

	bool movePlayer;
	bool isSimpleInteractable;
#pragma endregion


#pragma region Setter/Getter
public: 
	FORCEINLINE bool GetCanBeDetected() const { return canBeDetected; }
#pragma endregion


#pragma region Functions
public:
	AInteractableActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void InteractionStarted_Internal(ACustomPlayer* _player, USceneComponent* _playerPosition, bool& _inInteraction);
	virtual void InteractionCompleted_Internal(ACustomPlayer* _player, bool& _inInteraction);

	virtual void FirstInteraction(ACustomPlayer* _player, USceneComponent* _playerPosition);
	virtual void SecondInteraction(ACustomPlayer* _player);

	virtual void SetupPlayerInputs(ACustomPlayer* _player);
	virtual void TickRotateInteractionText();

	void RotateInteractionText(UTextRenderComponent* _textComp);

public:
	virtual void InteractionStarted(ACustomPlayer* _player, bool& _inInteraction);
	virtual void InteractionCompleted(ACustomPlayer* _player, bool& _inInteraction);

	virtual void DetectInteractable(ACustomPlayer* _player, const bool _isDetected);
#pragma endregion
};
