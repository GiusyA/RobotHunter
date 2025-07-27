#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RobotHunter/Utils/CustomMacros.h"
#include "RobotHunter/Utils/CustomEnum.h"
#include "RobotHunter/DataAsset/Input/InputConfigDA.h"
#include "RobotHunter/Component/Interaction/InteractionComponent.h"
#include "RobotHunter/Component/Camera/CustomCameraComponent.h"
#include "RobotHunter/Component/FSM/Player/PlayerFSMComponent.h"
#include "CustomPlayer.generated.h"

UCLASS()
class ROBOTHUNTER_API ACustomPlayer : public ACharacter
{
	GENERATED_BODY()


#pragma region Properties
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMove, ACustomPlayer*, player, const FVector&, movement);


	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug")
	bool useRealtime;


#pragma region Input
	UPROPERTY(EditAnywhere, Category = "Custom Property|Input")
	TObjectPtr<UInputConfigDA> inputConfig;

	UPROPERTY()
	TObjectPtr<UInputMappingContext> currentInputContext;

	bool inputEnabled;
	bool isAiming;
#pragma endregion


#pragma region Component


#pragma region Camera
	UPROPERTY(EditAnywhere, Category = "Custom Property|Component")
	TObjectPtr<UCustomCameraComponent> customCameraComponent;

	FTimerDelegate endBlendCameraDelegate;
#pragma endregion


	UPROPERTY(EditAnywhere, Category = "Custom Property|Component")
	TObjectPtr<UInteractionComponent> interactionComponent;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Component")
	TObjectPtr<UPlayerFSMComponent> fsmComponent;
#pragma endregion


	//Quick access to GetMesh()
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> playerMesh;

	float meshBaseYaw;


	FOnMove onMove;
#pragma endregion


#pragma region Setter/Getter
public:

#pragma region Input
	FORCEINLINE void SetInputEnabled(const bool _enabled) { inputEnabled = _enabled; }

	FORCEINLINE UInputConfigDA* GetInputConfig() const { return inputConfig; }
	FORCEINLINE UInputMappingContext* GetCurrentInputContext() const { return currentInputContext; }
#pragma endregion

#pragma region Component
	FORCEINLINE UCustomCameraComponent* GetCustomCameraComponent() const { return customCameraComponent; }
	FORCEINLINE UInteractionComponent* GetInteractionComponent() const { return interactionComponent; }
	FORCEINLINE UPlayerFSMComponent* GetFSMComponent() const { return fsmComponent; }
#pragma endregion

#pragma region FSM
	FORCEINLINE void SetFSMBool(const EPlayerBool& _index, const bool _newValue)
	{
		if (fsmComponent)
			fsmComponent->SetBool(_index, _newValue);
	}

	FORCEINLINE bool GetFSMBool(const EPlayerBool& _index) const
	{
		if (fsmComponent)
			return fsmComponent->GetBool(_index);

		return false;
	}
#pragma endregion


	FORCEINLINE FRotator GetCameraRelativeRotation() const
	{
		return customCameraComponent ? customCameraComponent->GetRelativeRotation()
			: FRotator(0.0f);
	}

	FORCEINLINE FRotator GetMeshRotation() const
	{
		return playerMesh ? playerMesh->GetComponentRotation()
				: FRotator(0.0f);
	}

	FORCEINLINE FOnMove& GetOnMove() { return onMove; }
#pragma endregion

#pragma region Functions
public:
	ACustomPlayer();

private:
	void SetMeshRotation(const FRotator& _rotation);

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual bool ShouldTickIfViewportsOnly() const override;

#pragma region Input
	void MoveTriggered(const FInputActionValue& _value);
	void RotateCameraTriggered(const FInputActionValue& _value);
	void JumpStarted(const FInputActionValue& _value);

	void InteractStarted(const FInputActionValue& _value);
	void InteractCompleted(const FInputActionValue& _value);

	void AimStarted(const FInputActionValue& _value);

public:
	void EnableInputContext(UInputMappingContext* _context);
	void DisableCurrentInputContext();
#pragma endregion

	void SetActorRotation(const FRotator& _rotation, const bool _updateCamera = false);

	void UpdateCameraCurrentSettings(const ECameraKey& _key);
	void SetCameraViewWithBlend(AActor* _newView, const float _blendSpeed);
#pragma endregion
};
