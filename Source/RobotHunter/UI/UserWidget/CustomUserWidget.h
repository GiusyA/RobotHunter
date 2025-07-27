#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RobotHunter/Utils/CustomEnum.h"
#include "RobotHunter/Utils/CustomMacros.h"
#include "CustomUserWidget.generated.h"

UCLASS()
class ROBOTHUNTER_API UCustomUserWidget : public UUserWidget
{
	GENERATED_BODY()
	

#pragma region Properties
protected:
	UPROPERTY(EditAnywhere, Category = "Custom Property")
	TEnumAsByte<EWidgetType> type;
#pragma endregion


#pragma region Setter/Getter
public:
	FORCEINLINE TEnumAsByte<EWidgetType> GetType() const { return type; }
#pragma endregion


#pragma region Functions
public:
	UCustomUserWidget(const FObjectInitializer& ObjectInitializer);

protected:
	FText CreateFTextFromFloat(const float _value, const int _maximumFractionalDigits = 0);
#pragma endregion
};
