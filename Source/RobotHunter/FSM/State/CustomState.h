#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RobotHunter/FSM/Transition/CustomTransition.h"
#include "CustomState.generated.h"

class UCustomFSM;

UCLASS(Blueprintable, BlueprintType)
class ROBOTHUNTER_API UCustomState : public UObject
{
	GENERATED_BODY()


#pragma region Properties
protected:
	UPROPERTY(EditAnywhere, Category = "Custom Property|Transitions")
	TArray<TSubclassOf<UCustomTransition>> transitionsReference;

	UPROPERTY()
	TObjectPtr<UCustomFSM> fsm;

	UPROPERTY()
	TArray<UCustomTransition*> transitions;
#pragma endregion


#pragma region Setter/Getter
public:
	FORCEINLINE void SetFSM(UCustomFSM* _fsm) { fsm = _fsm; }
#pragma endregion


#pragma region Functions
public:
	UCustomState();

private:
	void CheckForValidTransition();

public:
	void InitTransitions();

	virtual void Update();

	virtual void Enter() {}
	virtual void Exit() {}
#pragma endregion
};
