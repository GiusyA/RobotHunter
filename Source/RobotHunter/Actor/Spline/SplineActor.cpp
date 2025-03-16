#include "SplineActor.h"

ASplineActor::ASplineActor()
{
	spline = CREATE_DEFAULT_SUBOBJECT(USplineComponent, "Spline");
	SETUP_ATTACHMENT(spline, RootComponent);

	debugSphereRadius = 10.f;

	useCustomDistanceTool = false;
	distanceBetweenPointsTool = 100.0f;
	numberOfPointsTool = 10;
}


void ASplineActor::RemoveAllPoints() const
{
	const int _pointsCount = GetNumberOfPoints();

	if (_pointsCount > 0)
	{
		for (int i = _pointsCount - 1; i > 0; i--)
			spline->RemoveSplinePoint(i);
	}
}

void ASplineActor::AddPoints(const TArray<FSplinePoint>& _pointsData) const
{
	if (spline && !_pointsData.IsEmpty())
	{
		const int _pointsDataCount = _pointsData.Num();

		for (int i = 0; i < _pointsDataCount; i++)
			spline->AddPoint(_pointsData[i]);
	}
}

void ASplineActor::ModifyPointFromDistance(FSplinePoint& _outPoint, const float _distance, const int _index)
{
	if (spline)
	{
		const FVector _tangent = spline->GetTangentAtDistanceAlongSpline(_distance, ESplineCoordinateSpace::World);
		_outPoint.ArriveTangent = _tangent;
		_outPoint.LeaveTangent = _tangent;

		_outPoint.Position = spline->GetLocationAtDistanceAlongSpline(_distance, ESplineCoordinateSpace::Local);
		_outPoint.InputKey = _index;

		_outPoint.Type = ESplinePointType::Curve;
	}
}


void ASplineActor::DrawDebugTool() const
{
	Super::DrawDebugTool();

	if (spline)
	{
		FVector _currentLoc = FVector();
		const float _distanceBetweenPoints = useCustomDistanceTool ? distanceBetweenPointsTool 
												: spline->GetSplineLength() / (numberOfPointsTool - 1);

		for (int i = 0; i < numberOfPointsTool; i++)
		{
			_currentLoc = GetLocationAtDistance(_distanceBetweenPoints * i);
			DRAW_DEBUG_SPHERE(_currentLoc, debugSphereRadius, 10, FColor::Blue);
		}
	}
}


void ASplineActor::RefreshPoints()
{
	if (spline)
	{
		const float _splineLength = spline->GetSplineLength();

		const float _distanceBetweenPoints = useCustomDistanceTool ? distanceBetweenPointsTool 
												: _splineLength / (numberOfPointsTool - 1);

		float _currentDistance = 0.0f;

		FSplinePoint _point = FSplinePoint();
		TArray<FSplinePoint> _pointsData = TArray<FSplinePoint>();

		bool _endLoop = false;

		for (int i = 1; i < numberOfPointsTool; i++)
		{
			_currentDistance = _distanceBetweenPoints * i;

			if (_currentDistance > _splineLength)
			{
				_currentDistance = _splineLength;
				_endLoop = true;
			}

			ModifyPointFromDistance(_point, _currentDistance, i);
			_pointsData.Add(_point);

			if (_endLoop)
				break;
		}

		RemoveAllPoints();
		AddPoints(_pointsData);
	}
}
