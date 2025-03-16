#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Actor/Interactable/InteractableActor.h"
#include "RobotHunter/Actor/Spline/Rail/RailActor.h"
#include "RailLeverActor.generated.h"

UCLASS()
class ROBOTHUNTER_API ARailLeverActor : public AInteractableActor
{
	GENERATED_BODY()


#pragma region Properties


#pragma region Mesh
	UPROPERTY(EditAnywhere, Category = "Custom Property|Mesh")
	TObjectPtr<UStaticMeshComponent> baseMesh;


	UPROPERTY(EditAnywhere, Category = "Custom Property|Mesh")
	TObjectPtr<USceneComponent> baseLever;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Mesh")
	TObjectPtr<UStaticMeshComponent> meshLever;


	UPROPERTY(EditAnywhere, Category = "Custom Property|Mesh", meta = (UIMin = 0.0f, ClampMin = 0.0f, UIMax = 90.0f, ClampMax = 90.0f))
	float meshLeverAngle;
#pragma endregion
	

	UPROPERTY(EditAnywhere, Category = "Custom Property|Rail")
	TSubclassOf<ARailActor> railReference;


	UPROPERTY(EditAnywhere, Category = "Custom Property|Rail")
	TArray<ARailActor*> returnRails;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Rail")
	TArray<ARailActor*> departureRails;


	UPROPERTY(EditAnywhere, Category = "Custom Property|Rail|Index", meta = (UIMin = 0, ClampMin = 0))
	int currentIndexReturn;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Rail|Index", meta = (UIMin = 0, ClampMin = 0))
	int currentIndexDeparture;
#pragma endregion


#pragma region Setter/Getter
public:
	FORCEINLINE int GetCurrentIndex(const bool _onReturn) const 
	{ 
		return _onReturn ? currentIndexReturn 
				: currentIndexDeparture;
	}

	FORCEINLINE ARailActor* GetRailFromIndex(const int _index, const bool _isReturnRail) const
	{
		if (_index >= 0)
		{
			const TArray<ARailActor*> _rails = _isReturnRail ? returnRails 
												: departureRails;

			if (!_rails.IsEmpty() && _index < _rails.Num())
				return _rails[_index];
		}

		return nullptr;
	}

	FORCEINLINE TArray<ARailActor*> GetRails(const bool _onReturn) const 
	{ 
		return _onReturn ? returnRails 
				: departureRails;
	}
#pragma endregion


#pragma region Functions
public:
	ARailLeverActor();

private:
	void DrawDebugRails(const bool _onReturn) const;

	/// <summary>
	/// Remove nullptr rails from the array.
	/// </summary>
	void RefreshRails(TArray<ARailActor*>& _railsToRefresh);

protected:
	virtual void BeginPlay() override;
	virtual void DrawDebug() const override;

	virtual void InteractionStarted(ACustomPlayer* _player, bool& _inInteraction) override;
	virtual void InteractionCompleted(ACustomPlayer* _player, bool& _inInteraction) override;

public:
	UFUNCTION(CallInEditor) void SpawnDepartureRail();
	UFUNCTION(CallInEditor) void SpawnReturnRail();

	void SpawnRail(const bool _isReturnRail);
	void AddRail(ARailActor* _rail, const bool _isReturnRail);
	void UpdateBlockedRails(const bool _onReturn, const bool _updateIndex = true);

	ARailActor* GetNextRail(const bool _onReturn) const;
#pragma endregion
};
