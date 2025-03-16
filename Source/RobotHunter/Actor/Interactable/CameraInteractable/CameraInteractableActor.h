#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Actor/Interactable/InteractableActor.h"
#include "RobotHunter/Component/Camera/CustomCameraComponent.h"
#include "CameraInteractableActor.generated.h"

UCLASS()
class ROBOTHUNTER_API ACameraInteractableActor : public AInteractableActor
{
	GENERATED_BODY()


#pragma region Properties
protected:
	UPROPERTY(EditAnywhere, Category = "Custom Property|Component")
	TObjectPtr<UCustomCameraComponent> customCameraComponent;
#pragma endregion
	

#pragma region Functions
public:
	ACameraInteractableActor();

protected:
	virtual void FirstInteraction(ACustomPlayer* _player, USceneComponent* _playerPosition) override;
	virtual void SecondInteraction(ACustomPlayer* _player) override;
#pragma endregion
};
