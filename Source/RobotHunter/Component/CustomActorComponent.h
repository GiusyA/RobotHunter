#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RobotHunter/Utils/CustomMacros.h"
#include "RobotHunter/Utils/CustomEnum.h"
#include "CustomActorComponent.generated.h"


UCLASS()
class ROBOTHUNTER_API UCustomActorComponent : public UActorComponent
{
	GENERATED_BODY()


#pragma region Properties
protected:
	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug")
	bool useDebug;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Debug")
	bool useDebugTool;


	bool isRuntime;
#pragma endregion


#pragma region Functions
public:	
	UCustomActorComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void DrawDebug() const {}
	virtual void DrawDebugTool() const {}

public:
	virtual void SetupAttachment(USceneComponent* _sceneComp) const {}
#pragma endregion
};
