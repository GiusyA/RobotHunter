#include "FSMComponent.h"

UFSMComponent::UFSMComponent()
{
	initialFSMReference = nullptr;
	currentFSM = nullptr;
}


void UFSMComponent::Update()
{
	if (currentFSM)
		currentFSM->Update();
}

void UFSMComponent::Stop()
{
	if (currentFSM)
		currentFSM->Stop();
}


void UFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Update();
}

void UFSMComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	Stop();
}


void UFSMComponent::Start()
{
	if (initialFSMReference)
	{
		currentFSM = NEW_OBJECT(UCustomFSM, initialFSMReference);

		if (currentFSM)
		{
			currentFSM->SetFSMComponent(this);
			currentFSM->Start();
		}
	}
}
