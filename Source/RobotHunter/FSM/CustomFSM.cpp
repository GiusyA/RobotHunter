#include "CustomFSM.h"
#include "RobotHunter/Component/FSM/FSMComponent.h"

UCustomFSM::UCustomFSM()
{
	initialState = nullptr;

	currentState = nullptr;
	allStates = TMap<FString, UCustomState*>();

	fsmComponent = nullptr;
}


void UCustomFSM::CreateState(const FString& _key, const TSubclassOf<UCustomState>& _state)
{
	currentState = NEW_OBJECT(UCustomState, _state);

	if (currentState)
	{
		currentState->SetFSM(this);
		currentState->InitTransitions();
		allStates.Add(_key, currentState);
	}
}


void UCustomFSM::Start()
{
	ChangeState(initialState);
}

void UCustomFSM::Update()
{
	if (currentState)
		currentState->Update();
}

void UCustomFSM::Stop()
{
	if (currentState)
	{
		currentState->Exit();
		currentState = nullptr;
	}
}


void UCustomFSM::ChangeState(const TSubclassOf<UCustomState>& _state)
{
	if (_state)
	{
		const FString _stateKey = _state->GetName();

		if (!allStates.Contains(_stateKey))
			CreateState(_stateKey, _state);
		else
			currentState = allStates[_stateKey];

		if (currentState)
			currentState->Enter();
	}
}

AActor* UCustomFSM::GetOwner() const
{
	if (fsmComponent)
		return fsmComponent->GetOwner();

	return nullptr;
}
