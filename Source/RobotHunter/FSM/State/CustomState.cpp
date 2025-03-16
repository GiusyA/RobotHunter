#include "CustomState.h"
#include "RobotHunter/FSM/CustomFSM.h"

UCustomState::UCustomState()
{
	transitionsReference = TArray<TSubclassOf<UCustomTransition>>();
	fsm = nullptr;
	transitions = TArray<UCustomTransition*>();
}


void UCustomState::CheckForValidTransition()
{
	if (!transitions.IsEmpty())
	{
		const int _num = transitions.Num();
		UCustomTransition* _transition = nullptr;

		for (size_t i = 0; i < _num; i++)
		{
			_transition = transitions[i];

			if (_transition && _transition->IsValidTransition())
			{
				if (fsm)
				{
					Exit();
					fsm->ChangeState(_transition->GetNextState());
					return;
				}
			}
		}
	}
}


void UCustomState::InitTransitions()
{
	const int _num = transitionsReference.Num(); 
	UCustomTransition* _transition = nullptr;
	TSubclassOf<UCustomTransition> _transitionReference = nullptr;

	for (size_t i = 0; i < _num; i++)
	{
		_transitionReference = transitionsReference[i];

		if (_transitionReference)
		{
			_transition = NEW_OBJECT(UCustomTransition, _transitionReference);

			if (_transition)
			{
				_transition->SetFSM(fsm);
				_transition->InitTransition();
				transitions.Add(_transition);
			}
		}
	}
}


void UCustomState::Update()
{
	CheckForValidTransition();
}
