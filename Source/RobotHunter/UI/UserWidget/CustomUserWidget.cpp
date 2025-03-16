#include "CustomUserWidget.h"

UCustomUserWidget::UCustomUserWidget(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	type = EWidgetType::DefaultWidgetType;
}
