#include "CustomHUD.h"

ACustomHUD::ACustomHUD()
{
	widgetReferences = TArray<TSubclassOf<UCustomUserWidget>>();
	widgets = TMap<TEnumAsByte<EWidgetType>, UCustomUserWidget*>();

	currentWidgetTypes = TArray<TEnumAsByte<EWidgetType>>();
}


void ACustomHUD::InitializeWidgets()
{
	if (!widgetReferences.IsEmpty())
	{
		const int _num = widgetReferences.Num();
		UCustomUserWidget* _widget = nullptr;

		for (int i = 0; i < _num; i++)
		{
			_widget = CREATE_CUSTOM_WIDGET(widgetReferences[i]);

			if (_widget)
			{
				_widget->SetVisibility(ESlateVisibility::Hidden);
				_widget->AddToViewport();
				widgets.Add(_widget->GetType(), _widget);
			}
		}
	}
}


void ACustomHUD::BeginPlay()
{
	Super::BeginPlay();
	InitializeWidgets();
}


void ACustomHUD::SetWidgetVisibility(const bool _isVisible, const EWidgetType& _type)
{
	const bool _isDefaultType = _type == EWidgetType::DefaultWidgetType;

	if (!_isDefaultType)
	{
		UCustomUserWidget* _widget = GetWidget(_type);

		if (_widget)
		{
			ESlateVisibility _visibility = ESlateVisibility::Hidden;

			if (_isVisible)
			{
				currentWidgetTypes.Add(_type);
				_visibility = ESlateVisibility::Visible;
			}
			else
				currentWidgetTypes.Remove(_type);

			_widget->SetVisibility(_visibility);
		}
	}
}

void ACustomHUD::SetWidgetsVisibility(const bool _isVisible, const TArray<TEnumAsByte<EWidgetType>>& _types)
{
	if (!_types.IsEmpty())
	{
		for (const EWidgetType _type : _types)
			SetWidgetVisibility(_isVisible, _type);
	}
}
