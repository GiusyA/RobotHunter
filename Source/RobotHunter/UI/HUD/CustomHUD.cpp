#include "CustomHUD.h"

ACustomHUD::ACustomHUD()
{
	currentWidgetType = EWidgetType::DefaultWidgetType;
	widgetReferences = TArray<TSubclassOf<UCustomUserWidget>>();
	widgets = TMap<TEnumAsByte<EWidgetType>, UCustomUserWidget*>();
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


void ACustomHUD::SetCurrentWidget(const EWidgetType& _type)
{
	const bool _default = _type == EWidgetType::DefaultWidgetType;
	UCustomUserWidget* _widget = GetCurrentWidget();

	if (_widget)
		_widget->SetVisibility(ESlateVisibility::Hidden);
	
	if (!_default && widgets.Contains(_type))
	{
		_widget = widgets.FindRef(_type);

		if (_widget)
			_widget->SetVisibility(ESlateVisibility::Visible);
	}
	
	currentWidgetType = _type;
}
