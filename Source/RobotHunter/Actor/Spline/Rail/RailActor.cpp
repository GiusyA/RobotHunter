#include "RailActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "RobotHunter/Actor/Interactable/Rail/Lever/RailLeverActor.h"

ARailActor::ARailActor()
{
	arrowComponent = CREATE_DEFAULT_SUBOBJECT(UArrowComponent, "Arrow");
	SETUP_ATTACHMENT(arrowComponent, RootComponent);

	leverReference = nullptr;
	departureLever = nullptr;
	returnLever = nullptr;

	departureStopIndex = 0;
	returnStopIndex = 0;

	isBlockedOnDepartureExit = false;
	isBlockedOnReturnExit = false;
}

void ARailActor::UpdateArrowPosition() const
{
	if (arrowComponent)
	{
		const FVector _pointLoc = GetStopLocation(false);
		arrowComponent->SetWorldTransform(FTransform(_pointLoc));

		const FVector _targetLoc = GetLocationAtPointIndex(departureStopIndex);

		const FRotator _lookAt = UKismetMathLibrary::FindLookAtRotation(_pointLoc, _targetLoc);
		arrowComponent->SetWorldRotation(_lookAt);
	}
}

ARailLeverActor* ARailActor::SpawnLever(const bool _isReturnLever)
{
	if (leverReference)
	{
		const FVector _spawnLoc = GetLocationAtPointIndex(GetStopIndex(_isReturnLever)) + FVector(10.0f, 10.0f, 0.0f);
		ARailLeverActor* _lever = SPAWN_ACTOR_LOCATION(ARailLeverActor, leverReference, _spawnLoc);

		if (_lever)
		{
			_lever->AddRail(this, !_isReturnLever);
			_lever->SpawnRail(_isReturnLever);
			return _lever;
		}
	}

	return nullptr;
}

void ARailActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!isRuntime)
		UpdateArrowPosition();
}

void ARailActor::DrawDebug() const
{
	Super::DrawDebug();

	FVector _sphereLoc = FVector();

	_sphereLoc = GetLocationAtPointIndex(GetStopIndex(false));
	DRAW_DEBUG_SPHERE(_sphereLoc, debugSphereRadius, 10, FColor::Orange);

	_sphereLoc = GetLocationAtPointIndex(GetStopIndex(true));
	DRAW_DEBUG_SPHERE(_sphereLoc, debugSphereRadius, 10, FColor::Magenta);
}

void ARailActor::SpawnDepartureLever()
{
	if (!departureLever)
		departureLever = SpawnLever(false);
}

void ARailActor::SpawnReturnLever()
{
	if (!returnLever)
		returnLever = SpawnLever(true);
}
