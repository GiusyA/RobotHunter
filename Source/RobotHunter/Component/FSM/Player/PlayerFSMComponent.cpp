#include "PlayerFSMComponent.h"

UPlayerFSMComponent::UPlayerFSMComponent()
{
	fsmBool = TArray<bool>();
}

void UPlayerFSMComponent::InitAllBool()
{
	const int _boolNum = EPlayerBool::CountPlayerBool - 1;

	for (int i = 0; i < _boolNum; i++)
		fsmBool.Add(false);
}

void UPlayerFSMComponent::Start()
{
	InitAllBool();
	Super::Start();
}
