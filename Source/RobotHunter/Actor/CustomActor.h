#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RobotHunter/Utils/CustomMacros.h"
#include "RobotHunter/Utils/CustomEnum.h"
#include "RobotHunter/UI/HUD/CustomHUD.h"
#include "Components/ShapeComponent.h"
#include "CustomActor.generated.h"

UCLASS()
class ROBOTHUNTER_API ACustomActor : public AActor
{
	GENERATED_BODY()


#pragma region Properties
protected:
	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug")
	bool useRealtime;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug")
	bool useDebug;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug")
	bool useDebugTool;


	UPROPERTY(EditAnywhere, Category = "Custom Property|Trigger")
	TObjectPtr<UShapeComponent> trigger;


	UPROPERTY()
	TObjectPtr<ACustomHUD> hud;


	bool isRuntime;
#pragma endregion
	

#pragma region Functions
public:	
	ACustomActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual bool ShouldTickIfViewportsOnly() const override;

	virtual void DrawDebug() const {}
	virtual void DrawDebugTool() const {}

	virtual void PrintDebug() const {}

	UFUNCTION() virtual void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
		bool bFromSweep, const FHitResult& SweepResult) { }

	UFUNCTION() virtual void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {}

	void RetrieveHUD();
#pragma endregion
};
