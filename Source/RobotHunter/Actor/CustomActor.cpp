#include "CustomActor.h"

ACustomActor::ACustomActor()
{
#if WITH_EDITOR 
	PrimaryActorTick.bStartWithTickEnabled = true;
#endif

	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CREATE_DEFAULT_SUBOBJECT(USceneComponent, "Root");

	useRealtime = false;
	useDebug = false;
	useDebugTool = false;

	isRuntime = false;
}


void ACustomActor::BeginPlay()
{
	Super::BeginPlay();
	isRuntime = true;
}

void ACustomActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (useDebug)
	{
		DrawDebug();
		PrintDebug();
	}

	if (useDebugTool)
		DrawDebugTool();
}

bool ACustomActor::ShouldTickIfViewportsOnly() const
{
	return useRealtime;
}

