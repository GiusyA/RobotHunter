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

	hud = nullptr;

	isRuntime = false;
}


void ACustomActor::BeginPlay()
{
	Super::BeginPlay();

	isRuntime = true;
	RetrieveHUD();

	if (trigger)
	{
		trigger->OnComponentBeginOverlap.AddDynamic(this, &ACustomActor::OnTriggerBeginOverlap);
		trigger->OnComponentEndOverlap.AddDynamic(this, &ACustomActor::OnTriggerEndOverlap);
	}
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


void ACustomActor::RetrieveHUD()
{
	if (!hud)
	{
		APlayerController* _controller = FIRST_PLAYER_CONTROLLER;

		if (_controller)
			hud = CAST(ACustomHUD, _controller->GetHUD());
	}
}
