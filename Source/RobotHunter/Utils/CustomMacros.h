#pragma once

#include "Kismet/KismetSystemLibrary.h"

#define WORLD GetWorld()
#define DELTA_TIME GetWorld()->DeltaTimeSeconds

#define FIRST_PLAYER_CONTROLLER GetWorld()->GetFirstPlayerController()
#define TIMER_MANAGER GetWorld()->GetTimerManager()

#define NEW_OBJECT(objectType, reference) NewObject<objectType>(this, reference)
#define NEW_OBJECT_OWNER(objectType, owner, reference) NewObject<objectType>(owner, reference)

#define CREATE_DEFAULT_SUBOBJECT(classType, name) CreateDefaultSubobject<classType>(name)
#define SETUP_ATTACHMENT(toAttach, attachTo) if (toAttach) \
												 toAttach->SetupAttachment(attachTo);

#define CREATE_CUSTOM_WIDGET(widgetType) CreateWidget<UCustomUserWidget>(GetWorld(), widgetType)

#define SPAWN_ACTOR(classType, actorType) GetWorld()->SpawnActor<classType>(actorType)
#define SPAWN_ACTOR_LOCATION(classType, actorType, location) GetWorld()->SpawnActor<classType>(actorType, FTransform(location))

#define CAST(classType, object) Cast<classType>(object)


#pragma region Clamp
#define CLAMP(toClamp, min, max) toClamp = toClamp < min ? min \
											: toClamp > max ? max \
											: toClamp;

#define CLAMP_LOOP(toClamp, min, max) toClamp = toClamp < min ? max \
												: toClamp > max ? min \
												: toClamp;

#define CONST_CLAMP(value, min, max) value < min ? min \
									: value > max ? max \
									: value

#define CLAMP_ANGLE(angle) angle = angle < 0.0f ? 0.0f \
									: angle > 360.0f ? 360.0f \
									: angle;
#pragma endregion


#pragma region Debug

#pragma region Draw
#define DRAW_DEBUG_SPHERE(center, radius, segments, color) DrawDebugSphere(GetWorld(), center, radius, segments, color);
#define DRAW_DEBUG_LINE(lineStart, lineEnd, color) DrawDebugLine(GetWorld(), lineStart, lineEnd, color);
#pragma endregion

#pragma region Print
#define PRINT_SCREEN(toPrint, color, duration) UKismetSystemLibrary::PrintString(this, toPrint, true, false, color, duration)
#define PRINT_SCREEN_FLOAT(float, color, duration) UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(float), true, false, color, duration)
#define PRINT_SCREEN_WITH_FLOAT(toPrint, float, color, duration) UKismetSystemLibrary::PrintString(this, toPrint + FString::SanitizeFloat(float), true, false, color, duration)

#define PRINT_SCREEN_TICK(toPrint, color) UKismetSystemLibrary::PrintString(this, toPrint, true, false, color, GetWorld()->DeltaTimeSeconds)
#define PRINT_SCREEN_FLOAT_TICK(float, color) UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(float), true, false, color, GetWorld()->DeltaTimeSeconds)
#define PRINT_SCREEN_WITH_FLOAT_TICK(toPrint, float, color) UKismetSystemLibrary::PrintString(this, toPrint + FString::SanitizeFloat(float), true, false, color, GetWorld()->DeltaTimeSeconds)
#pragma endregion

#pragma endregion