#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Component/FSM/FSMComponent.h"
#include "PlayerFSMComponent.generated.h"


UCLASS()
class ROBOTHUNTER_API UPlayerFSMComponent : public UFSMComponent
{
	GENERATED_BODY()
	

#pragma region Properties
	UPROPERTY(VisibleAnywhere)
	TArray<bool> fsmBool;
#pragma endregion


#pragma region Setter/Getter
public:
	FORCEINLINE void SetBool(const EPlayerBool& _index, const bool _newValue)
	{
		if (_index >= 0 && _index < EPlayerBool::DefaultPlayerBool)
			fsmBool[_index] = _newValue;
	}

	FORCEINLINE bool GetBool(const EPlayerBool& _index)
	{
		if (_index >= 0 && _index < EPlayerBool::DefaultPlayerBool)
			return fsmBool[_index];

		return false;
	}
#pragma endregion


#pragma region Functions
public:
	UPlayerFSMComponent();

private:
	void InitAllBool();

public:
	virtual void Start() override;
#pragma endregion
};
