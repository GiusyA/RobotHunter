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
	TArray<TSubclassOf<UCustomUserWidget>> widgetReferences;

	UPROPERTY()
	TMap<TEnumAsByte<EWidgetType>, UCustomUserWidget*> widgets;

	UPROPERTY()
	TArray<TEnumAsByte<EWidgetType>> currentWidgetTypes;
#pragma endregion


#pragma region Setter/Getter
public:
	FORCEINLINE UCustomUserWidget* GetWidget(const EWidgetType& _type) const
	{
		if (widgets.Contains(_type))
			return widgets.FindRef(_type);

		return nullptr;
	}

	FORCEINLINE TArray<UCustomUserWidget*> GetCurrentWidgets() const
	{
		TArray<UCustomUserWidget*> _currentWidgets = TArray<UCustomUserWidget*>();

		if (!currentWidgetTypes.IsEmpty())
		{
			UCustomUserWidget* _widget = nullptr;

			for (const EWidgetType _type : currentWidgetTypes)
			{
				_widget = GetWidget(_type);

				if (_widget)
					_currentWidgets.Add(_widget);
			}
		}

		return _currentWidgets;
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
	void SetWidgetVisibility(const bool _isVisible, const EWidgetType& _type);
	void SetWidgetsVisibility(const bool _isVisible, const TArray<TEnumAsByte<EWidgetType>>& _types);
#pragma endregion
};
