#include "CustomTransition.h"

UCustomTransition::UCustomTransition()
{
	nextState = nullptr;
	fsm = nullptr;
}


bool UCustomTransition::IsValidTransition()
{
	return false;
}
