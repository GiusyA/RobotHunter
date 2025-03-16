#include "CustomActorComponent.h"

UCustomActorComponent::UCustomActorComponent()
{
#if WITH_EDITOR 
	PrimaryComponentTick.bStartWithTickEnabled = true;
#endif

	PrimaryComponentTick.bCanEverTick = true;

	useDebug = false;
	useDebugTool = false;

	isRuntime = false;
}


void UCustomActorComponent::BeginPlay()
{
	Super::BeginPlay();
	isRuntime = true;
}

void UCustomActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (useDebug)
		DrawDebug();

	if (useDebugTool)
		DrawDebugTool();
}

