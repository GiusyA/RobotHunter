#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Actor/Spline/SplineActor.h"
#include "Components/ArrowComponent.h"
#include "RailActor.generated.h"

/*The rail has two directions
- The departure direction (positive way) : toward the end of the rail (until distance == length of the rail)
- The return direction (negative way) : toward the beginning of the rail (until distance == 0)*/

class ARailLeverActor;

UCLASS()
class ROBOTHUNTER_API ARailActor : public ASplineActor
{
	GENERATED_BODY()


#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug")
	TObjectPtr<UArrowComponent> arrowComponent;


	UPROPERTY(EditAnywhere, Category = "Custom Property|Lever")
	TSubclassOf<ARailLeverActor> leverReference;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Lever")
	TObjectPtr<ARailLeverActor> departureLever;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Lever")
	TObjectPtr<ARailLeverActor> returnLever;


	/// <summary>
	/// On which point the actor should stop when the rail can't be left (departure direction).
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Custom Property|Stop", meta = (UIMin = 0, ClampMin = 0))
	int departureStopIndex;

	/// <summary>
	/// On which point the actor should stop when the rail can't be left (return direction).
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Custom Property|Stop", meta = (UIMin = 0, ClampMin = 0))
	int returnStopIndex;


	/// <summary>
	/// If the rail can be left (departure direction).
	/// </summary>
	bool isBlockedOnDepartureExit;
	/// <summary>
	/// If the way can be left (return direction).
	/// </summary>
	bool isBlockedOnReturnExit;
#pragma endregion


#pragma region Setter/Getter
public:
	FORCEINLINE void SetLever(ARailLeverActor* _lever, const bool _isReturnLever)
	{
		if (_isReturnLever)
			returnLever = _lever;
		else
			departureLever = _lever;
	}
	FORCEINLINE void SetIsBlocked(ARailLeverActor* _lever, const bool _isBlocked)
	{
		if (_lever == departureLever)
			isBlockedOnDepartureExit = _isBlocked;
		else if (_lever == returnLever)
			isBlockedOnReturnExit = _isBlocked;
	}


	FORCEINLINE bool GetIsBlocked(ARailLeverActor* _lever) const
	{
		return _lever == departureLever ? isBlockedOnDepartureExit
				: _lever == returnLever ? isBlockedOnReturnExit
				: true;
	}

	FORCEINLINE int GetStopIndex(const bool _onReturn) const
	{
		//Substract 1 to the index here so we don't have to do it in the blueprint
		const int _index = _onReturn ? (returnStopIndex - 1) 
							: (departureStopIndex - 1);

		const int _numberOfPoints = GetNumberOfPoints();
		return CONST_CLAMP(_index, 0, _numberOfPoints);
	}

	FORCEINLINE float GetDistanceAtStopLocation(const bool _onReturn) const
	{
		return GetDistanceAtLocation(GetStopLocation(_onReturn));
	}

	FORCEINLINE FVector GetStopLocation(const bool _onReturn) const
	{
		return GetLocationAtPointIndex(GetStopIndex(_onReturn));
	}

	FORCEINLINE ARailLeverActor* GetLever(const bool _onReturn) const 
	{
		return _onReturn ? returnLever 
				: departureLever; 
	}
#pragma endregion


#pragma region Functions
public:
	ARailActor();

private:
	void UpdateArrowPosition() const;

	ARailLeverActor* SpawnLever(const bool _isReturnLever);

protected:
	virtual void Tick(float DeltaTime) override;

	virtual void DrawDebug() const override;

public:
	UFUNCTION(CallInEditor) void SpawnDepartureLever();
	UFUNCTION(CallInEditor) void SpawnReturnLever();
#pragma endregion
};
