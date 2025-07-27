#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Utils/CustomEnum.h"
#include "Kismet/KismetMathLibrary.h"
#include "CustomLerp.generated.h"

USTRUCT()
struct FCustomLerp
{
	GENERATED_BODY()


private:
	bool isLerping;

	float currentDuration;

	float time;
	float alpha;

	float currentValueFloat;
	FVector currentValueVector;
	FRotator currentValueRotator;

public:
	UPROPERTY()
	TEnumAsByte<ECustomLerpType> type;

	UPROPERTY()
	TObjectPtr<UCurveFloat> curve;

	bool reverseCurve;

	float totalDuration;

	float baseValueFloat;
	float finalValueFloat;

	FVector baseValueVector;
	FVector finalValueVector;

	FRotator baseValueRotator;
	FRotator finalValueRotator;


public:
	bool GetIsLerping() const { return isLerping; }
	float GetCurrentValueFloat() const { return currentValueFloat; }
	FVector GetCurrentValueVector() const { return currentValueVector; }
	FRotator GetCurrentValueRotator() const { return currentValueRotator; }


public:
	FCustomLerp(const ECustomLerpType& _type)
	{
		isLerping = false;

		currentDuration = 0.0f;

		time = 0.0f;
		alpha = 0.0f;

		currentValueFloat = 0.0f;
		currentValueVector = FVector(0.0f);
		currentValueRotator = FRotator(0.0f);


		type = _type;

		curve = nullptr;

		reverseCurve = false;

		totalDuration = 0.0f;

		baseValueFloat = 0.0f;
		finalValueFloat = 0.0f;

		baseValueVector = FVector(0.0f);
		finalValueVector = FVector(0.0f);

		baseValueRotator = FRotator(0.0f);
		finalValueRotator = FRotator(0.0f);
	}

	FCustomLerp() : FCustomLerp(DefaultLerpType) {}


private:
	void UpdateAlpha(const float _deltaTime)
	{
		currentDuration += _deltaTime;
		isLerping = currentDuration < totalDuration;

		time = FMath::Lerp(0.0f, 1.0f, currentDuration / totalDuration);

		time = reverseCurve ? 1.0f - time
								: time;

		alpha = curve ? curve->GetFloatValue(time)
							: time;

		alpha = reverseCurve ? 1.0f - alpha 
								: alpha;
	}


	void LerpValueFloat()
	{
		if (!isLerping)
			currentValueFloat = finalValueFloat;
		else
			currentValueFloat = UKismetMathLibrary::Lerp(baseValueFloat, finalValueFloat, alpha);

	}

	void LerpValueVector()
	{
		if (!isLerping)
			currentValueVector = finalValueVector;
		else
			currentValueVector = UKismetMathLibrary::VLerp(baseValueVector, finalValueVector, alpha);
	}

	void LerpValueRotator()
	{
		if (!isLerping)
			currentValueRotator = finalValueRotator;
		else
			currentValueRotator = UKismetMathLibrary::RLerp(baseValueRotator, finalValueRotator, alpha, true);
	}


public:
	void StartLerp()
	{
		currentDuration = time = alpha = 0.0f;
		isLerping = true;
	}



	void LerpValue(const float _deltaTime)
	{
		if (isLerping)
		{
			UpdateAlpha(_deltaTime);

			switch (type)
			{
			case FloatLerpType:
			{
				LerpValueFloat();
				break;
			}

			case VectorLerpType:
			{
				LerpValueVector();
				break;
			}

			case RotatorLerpType:
			{
				LerpValueRotator();
				break;
			}

			default:
				break;
			}
		}
	}
};
