#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RobotHunter/Utils/CustomEnum.h"
#include "RobotHunter/Utils/CustomMacros.h"
#include "RobotHunter/UI/UserWidget/CustomUserWidget.h"
#include "CustomHUD.generated.h"

UCLASS()
class ROBOTHUNTER_API ACustomHUD : public AHUD
{
	GENERATED_BODY()
	

#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Custom Property|Widget")
	TEnumAsByte<EWidgetType> currentWidgetType;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Widget")
	TArray<TSubclassOf<UCustomUserWidget>> widgetReferences;

	UPROPERTY()
	TMap<TEnumAsByte<EWidgetType>, UCustomUserWidget*> widgets;
#pragma endregion


#pragma region Setter/Getter
public:
	FORCEINLINE UCustomUserWidget* GetWidget(const EWidgetType& _type) const
	{
		if (widgets.Contains(_type))
			return widgets.FindRef(_type);

		return nullptr;
	}

	FORCEINLINE UCustomUserWidget* GetCurrentWidget() const
	{
		return GetWidget(currentWidgetType);
	}
#pragma endregion


#pragma region Functions
public:
	ACustomHUD();

private:
	void InitializeWidgets();

protected:
	virtual void BeginPlay() override;

public:
	void SetCurrentWidget(const EWidgetType& _type);
#pragma endregion
};
