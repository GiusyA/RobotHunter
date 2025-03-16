#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/FSM/Transition/CustomTransition.h"
#include "CustomPlayerTransition.generated.h"

class ACustomPlayer;

UCLASS()
class ROBOTHUNTER_API UCustomPlayerTransition : public UCustomTransition
{
	GENERATED_BODY()


#pragma region Properties
protected:
	UPROPERTY(EditAnywhere, Category = "Custom Property")
	TMap<TEnumAsByte<EPlayerBool>, bool> toCheck;

	UPROPERTY()
	TObjectPtr<ACustomPlayer> player;
#pragma endregion


#pragma region Functions
public:
	UCustomPlayerTransition();

protected:
	void RetrievePlayer();

	bool CheckBool();

public:
	virtual void InitTransition() override;
	virtual bool IsValidTransition() override;
#pragma endregion
};
