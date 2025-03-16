#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Component/CustomActorComponent.h"
#include "RobotHunter/FSM/CustomFSM.h"
#include "FSMComponent.generated.h"

UCLASS()
class ROBOTHUNTER_API UFSMComponent : public UCustomActorComponent
{
	GENERATED_BODY()


#pragma region Properties
protected:
	UPROPERTY(EditAnywhere, Category = "Custom Property|FSM")
	TSubclassOf<UCustomFSM> initialFSMReference;

	UPROPERTY()
	TObjectPtr<UCustomFSM> currentFSM;
#pragma endregion


#pragma region Setter/Getter
public:
	FORCEINLINE TObjectPtr<UCustomFSM> GetCurrentFSM() const { return currentFSM; }
#pragma endregion


#pragma region Functions
public:
	UFSMComponent();

private:
	virtual void Update();
	virtual void Stop();

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Start();
#pragma endregion
};
