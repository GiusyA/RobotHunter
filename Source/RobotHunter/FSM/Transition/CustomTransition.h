#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RobotHunter/Utils/CustomMacros.h"
#include "RobotHunter/Utils/CustomEnum.h"
#include "CustomTransition.generated.h"

class UCustomState;
class UCustomFSM;

UCLASS(Blueprintable, BlueprintType)
class ROBOTHUNTER_API UCustomTransition : public UObject
{
	GENERATED_BODY()


#pragma region Properties
protected:
	UPROPERTY(EditAnywhere, Category = "Custom Property|State")
	TSubclassOf<UCustomState> nextState;

	UPROPERTY()
	TObjectPtr<UCustomFSM> fsm;
#pragma endregion


#pragma region Setter/Getter
public:
	FORCEINLINE void SetFSM(UCustomFSM* _fsm) { fsm = _fsm; }

	FORCEINLINE TSubclassOf<UCustomState> GetNextState() const { return nextState; }
#pragma endregion


#pragma region Functions
public:
	UCustomTransition();

public:
	virtual void InitTransition() {}
	virtual bool IsValidTransition();
#pragma endregion
};
