#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Actor/CustomActor.h"
#include "Components/SplineComponent.h"
#include "SplineActor.generated.h"


UCLASS()
class ROBOTHUNTER_API ASplineActor : public ACustomActor
{
	GENERATED_BODY()


#pragma region Properties
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Property", meta = (AllowPrivateAccess))
	TObjectPtr<USplineComponent> spline;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug", meta = (UIMin = 1.0f, ClampMin = 1.0f))
	float debugSphereRadius;


private:
	UPROPERTY(EditAnywhere, Category = "Custom Property|Tool|Points")
	bool useCustomDistanceTool;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Tool|Points", meta = (EditCondition = "useCustomDistance", EditConditionHides))
	float distanceBetweenPointsTool;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Tool|Points")
	float numberOfPointsTool;
#pragma endregion


#pragma region Setter/Getter
public:
	FORCEINLINE int GetNumberOfPoints() const 
	{ 
		return spline ? spline->GetNumberOfSplinePoints() 
				: 0;
	}


	FORCEINLINE float GetLength() const 
	{ 
		return spline ? spline->GetSplineLength() 
				: 0.0f;
	}

	FORCEINLINE float GetDistanceAtLocation(const FVector& _location) const
	{
		return spline ? spline->GetDistanceAlongSplineAtLocation(_location, ESplineCoordinateSpace::World) 
				: 0.0f;
	}


	FORCEINLINE FVector GetLocationAtDistance(const float _distance) const
	{
		return spline ? spline->GetLocationAtDistanceAlongSpline(_distance, ESplineCoordinateSpace::World) 
				: FVector(0.0f);
	}

	FORCEINLINE FVector GetLocationAtPointIndex(const int _index) const
	{
		return spline ? spline->GetLocationAtSplineInputKey(_index, ESplineCoordinateSpace::World) 
				: FVector(0.0f);
	}


	FORCEINLINE TObjectPtr<USplineComponent> GetSpline() const { return spline; }
#pragma endregion


#pragma region Functions
public:
	ASplineActor();

private:
	void RemoveAllPoints() const;
	void AddPoints(const TArray<FSplinePoint>& _pointsData) const;
	void ModifyPointFromDistance(FSplinePoint& _outPoint, const float _distance, const int _index);

protected:
	virtual void DrawDebugTool() const override;

public:
	UFUNCTION(CallInEditor) void RefreshPoints();
#pragma endregion
};
