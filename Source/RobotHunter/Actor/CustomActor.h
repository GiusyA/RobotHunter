#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RobotHunter/Utils/CustomMacros.h"
#include "RobotHunter/Utils/CustomEnum.h"
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
#pragma endregion
};
