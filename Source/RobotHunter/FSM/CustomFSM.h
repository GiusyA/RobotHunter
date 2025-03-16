#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RobotHunter/FSM/State/CustomState.h"
#include "RobotHunter/FSM/Transition/CustomTransition.h"
#include "CustomFSM.generated.h"

class UFSMComponent;

UCLASS(Blueprintable, BlueprintType)
class ROBOTHUNTER_API UCustomFSM : public UObject
{
	GENERATED_BODY()


#pragma region Properties
protected:
	UPROPERTY(EditAnywhere, Category = "Custom Property|State")
	TSubclassOf<UCustomState> initialState;

	UPROPERTY()
	TObjectPtr<UCustomState> currentState;

	UPROPERTY()
	TMap<FString, UCustomState*> allStates;

	UPROPERTY()
	TObjectPtr<UFSMComponent> fsmComponent;
#pragma endregion


#pragma region Setter/Getter
public:
	FORCEINLINE void SetFSMComponent(UFSMComponent* _fsmComponent) { fsmComponent = _fsmComponent; }

	FORCEINLINE TSubclassOf<UCustomState> GetInitialState() const { return initialState; }
	FORCEINLINE UCustomState* GetCurrentState() const { return currentState; }

	FORCEINLINE UFSMComponent* GetFSMComponent() const { return fsmComponent; }
#pragma endregion


#pragma region Functions
public:
	UCustomFSM();

private:
	void CreateState(const FString& _key, const TSubclassOf<UCustomState>& _state);

public:
	virtual void Start();
	virtual void Update();
	virtual void Stop();

	void ChangeState(const TSubclassOf<UCustomState>& _state);

	AActor* GetOwner() const;
#pragma endregion
};
