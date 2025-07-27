#include "CustomUserWidget.h"

UCustomUserWidget::UCustomUserWidget(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	type = EWidgetType::DefaultWidgetType;
}


FText UCustomUserWidget::CreateFTextFromFloat(const float _value, const int _maximumFractionalDigits)
{
	FNumberFormattingOptions _options = FNumberFormattingOptions();
	_options.MaximumFractionalDigits = _maximumFractionalDigits;

	const FText _newFText = FText().AsNumber(_value, &_options);
	return _newFText;
}
