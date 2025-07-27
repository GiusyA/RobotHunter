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
		const int _internal_index = _index - 1;

		if (_internal_index >= 0 && _internal_index < EPlayerBool::CountPlayerBool - 1)
			fsmBool[_internal_index] = _newValue;
	}

	FORCEINLINE bool GetBool(const EPlayerBool& _index)
	{
		const int _internal_index = _index - 1;

		if (_internal_index >= 0 && _internal_index < EPlayerBool::CountPlayerBool - 1)
			return fsmBool[_internal_index];

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
