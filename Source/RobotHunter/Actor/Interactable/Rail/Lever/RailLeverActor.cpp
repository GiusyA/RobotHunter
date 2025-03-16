#include "RailLeverActor.h"

ARailLeverActor::ARailLeverActor()
{
#pragma region Mesh
	baseMesh = CREATE_DEFAULT_SUBOBJECT(UStaticMeshComponent, "BaseMesh");
	SETUP_ATTACHMENT(baseMesh, RootComponent);

	baseLever = CREATE_DEFAULT_SUBOBJECT(USceneComponent, "BaseLever");
	SETUP_ATTACHMENT(baseLever, baseMesh);

	meshLever = CREATE_DEFAULT_SUBOBJECT(UStaticMeshComponent, "MeshLever");
	SETUP_ATTACHMENT(meshLever, baseLever);

	meshLeverAngle = 40.0f;
#pragma endregion

	railReference = nullptr;

	returnRails = TArray<ARailActor*>();
	departureRails = TArray<ARailActor*>();

	currentIndexReturn = 0;
	currentIndexDeparture = 0;
}


void ARailLeverActor::DrawDebugRails(const bool _onReturn) const
{
	const TArray<ARailActor*> _rails = _onReturn ? returnRails 
										: departureRails;

	if (!_rails.IsEmpty())
	{
		const int _currentIndex = _onReturn ? currentIndexReturn 
									: currentIndexDeparture;

		const FVector _actorLoc = GetActorLocation();
		FColor _debugColor = FColor();

		const int _railsNum = _rails.Num();
		ARailActor* _rail = nullptr;

		bool _isLeverRegistered = false;

		for (int i = 0; i < _railsNum; i++)
		{
			_rail = _rails[i];

			if (_rail)
			{
				//This works as a double check (visual feedback) — registered rails in the lever need to have the lever registered in them too
				_isLeverRegistered = _rail->GetLever(!_onReturn) == this;

				_debugColor = (!_isLeverRegistered) ? FColor::Red 
								: i == _currentIndex ? FColor::Green 
								: FColor::Blue;

				DRAW_DEBUG_LINE(_actorLoc, _rail->GetLocationAtPointIndex(_rail->GetStopIndex(!_onReturn)), _debugColor);
			}
		}
	}
}

void ARailLeverActor::RefreshRails(TArray<ARailActor*>& _railsToRefresh)
{
	if (!_railsToRefresh.IsEmpty())
	{
		TArray<ARailActor*> _rails = TArray<ARailActor*>();
		const int _num = _railsToRefresh.Num();
		ARailActor* _rail = nullptr;

		for (int i = 0; i < _num; i++)
		{
			_rail = _railsToRefresh[i];

			if (_rail)
				_rails.Add(_rail);
		}

		_railsToRefresh = _rails;
	}
}


void ARailLeverActor::BeginPlay()
{
	Super::BeginPlay();

	RefreshRails(departureRails);
	UpdateBlockedRails(false, false);

	RefreshRails(returnRails);
	UpdateBlockedRails(true, false);

	if (baseLever)
		baseLever->SetWorldRotation(FRotator(0.0f, 0.0f, meshLeverAngle));
}

void ARailLeverActor::DrawDebug() const
{
	Super::DrawDebug();

	DrawDebugRails(false);
	DrawDebugRails(true);
}

void ARailLeverActor::InteractionStarted(ACustomPlayer* _player, bool& _inInteraction)
{
	Super::InteractionStarted(_player, _inInteraction);

	UpdateBlockedRails(false);
	UpdateBlockedRails(true);

	if (baseLever)
		baseLever->SetWorldRotation(FRotator(0.0f, 0.0f, -meshLeverAngle));
}

void ARailLeverActor::InteractionCompleted(ACustomPlayer* _player, bool& _inInteraction)
{
	Super::InteractionCompleted(_player, _inInteraction);

	if (baseLever)
		baseLever->SetWorldRotation(FRotator(0.0f, 0.0f, meshLeverAngle));
}


void ARailLeverActor::SpawnDepartureRail()
{
	SpawnRail(false);
}

void ARailLeverActor::SpawnReturnRail()
{
	SpawnRail(true);
}

void ARailLeverActor::SpawnRail(const bool _isReturnRail)
{
	if (railReference)
	{
		const FVector _spawnLoc = GetActorLocation() + FVector(10.0f, 10.0f, 0.0f);
		ARailActor* _rail = SPAWN_ACTOR_LOCATION(ARailActor, railReference, _spawnLoc);

		if (_rail)
		{
			_rail->SetLever(this, !_isReturnRail);
			_rail->SetActorLocation(_spawnLoc);
			AddRail(_rail, _isReturnRail);
		}
	}
}

void ARailLeverActor::AddRail(ARailActor* _rail, const bool _isReturnRail)
{
	if (_rail)
	{
		TArray<ARailActor*>& _rails = _isReturnRail ? returnRails
										: departureRails;
		
		RefreshRails(_rails);
		_rails.Add(_rail);
	}
}

void ARailLeverActor::UpdateBlockedRails(const bool _onReturn, const bool _updateIndex)
{
	const TArray<ARailActor*> _rails = _onReturn ? returnRails
										: departureRails;

	if (!_rails.IsEmpty())
	{
		int& _currentIndex = _onReturn ? currentIndexReturn
								: currentIndexDeparture;

		const int _num = _rails.Num();
		bool _isBlocked = false;

		if (_updateIndex)
			_currentIndex += 1;

		CLAMP_LOOP(_currentIndex, 0, _num - 1);

		for (int i = 0; i < _num; i++)
		{
			if (_rails[i])
			{
				_isBlocked = (i == _currentIndex) ? false
								: true;

				_rails[i]->SetIsBlocked(this, _isBlocked);
			}
		}
	}
}

ARailActor* ARailLeverActor::GetNextRail(const bool _onReturn) const
{
	return _onReturn ? GetRailFromIndex(currentIndexReturn, true) 
			: GetRailFromIndex(currentIndexDeparture, false);
}
