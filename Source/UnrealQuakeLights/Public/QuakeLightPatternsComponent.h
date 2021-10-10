// Copyright 2020-2021 Solar Storm Interactive. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "QuakeLightPatternsComponent.generated.h"

// The baseline light patterns found in Quake and an addition in Half-Life 1
UENUM(BlueprintType)
enum class EQuakeLightPattern : uint8
{
	/** 0 normal "m" */
	Normal,
	/** 1 FLICKER (first variety) "mmnmmommommnonmmonqnmmo" */
	Flicker,
	/** 2 SLOW STRONG PULSE "abcdefghijklmnopqrstuvwxyzyxwvutsrqponmlkjihgfedcba" */
	SlowStrongPulse,
	/** 3 CANDLE (first variety) "mmmmmaaaaammmmmaaaaaabcdefgabcdefg" */
	Candle,
	/** 4 FAST STROBE "mamamamamama" */
	FastStrobe,
	/** 5 GENTLE PULSE 1 "jklmnopqrstuvwxyzyxwvutsrqponmlkj" */
	GentlePulse,
	/** 6 FLICKER (second variety) "nmonqnmomnmomomno" */
	Flicker2,
	/** 7 CANDLE (second variety) "mmmaaaabcdefgmmmmaaaammmaamm" */
	Candle2,
	/** 8 CANDLE (third variety) "mmmaaammmaaammmabcdefaaaammmmabcdefmmmaaaa" */
	Candle3,
	/** 9 SLOW STROBE (fourth variety) "aaaaaaaazzzzzzzz" */
	SlowStrobe,
	/** 10 FLUORESCENT FLICKER "mmamammmmammamamaaamammma" */
	FluorescentFlicker,
	/** 11 SLOW PULSE NOT FADE TO BLACK "abcdefghijklmnopqrrqponmlkjihgfedcba" */
	SlowPulseNotFadeToBlack,
	/** 12 UNDERWATER LIGHT MUTATION (Added for Half-Life 1) "mmnnmmnnnmmnn" */
	UnderwaterLightMutation,

	LAST_PATTERN_INDEX UMETA(Hidden),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuakeLightPatternChangedSignature, uint8, patternChar, float, patternFrac);

/**
 * A component that modifies light intensities of a light component in patterns originally defined in the 1996 game Quake.
 * If the light component is not set, it will choose the first found light component in the actor this component is placed in.
 * This component must be active to function.
 * This component starts by taking the baseline intensity for the light and shifts that based on the pattern.
 * If this component is deactivated, the light will be reverted to its original intensity.
 * If a new light is choosen (SetLightComponent) the original light will be reverted.
 */
UCLASS(ClassGroup=(Generator), meta=(BlueprintSpawnableComponent))
class UNREALQUAKELIGHTS_API UQuakeLightPatternsComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UQuakeLightPatternsComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;

	/**
	 * Set the light component to use
	 * @param lightComponent The light component to control
	 * @param overrideBaseIntensity If not -1.0f, this intensity will be used as a base instead of the one on the light component.
	 *								The intensity will still be reverted to the original intensity if the light component is unset
	 *								or this component is deactivated.
	 */
	UFUNCTION(BlueprintCallable, Category = "Quake Light")
	void SetLightComponent(ULightComponent* lightComponent, const float overrideBaseIntensity = -1.0f);

	/**
	 * Allows setting the custom pattern to use. Pass in an empty string to revert using a custom pattern back to using the baseline Pattern.
	 * Accepts a string of characters which fall within the ASCII range a-z.
	 * 'm' is normal light, 'a' is no light, 'z' is double bright
	 * Character which fall out of the range will be clamped.
	 */
	UFUNCTION(BlueprintCallable, Category = "Quake Light")
	void SetCustomPattern(const FString& pattern);
	
	/** The baseline pattern to use if no custom pattern is defined. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quake Light")
	EQuakeLightPattern Pattern;

	/// Called each tick with the output pattern in char and float intensity
	UPROPERTY(BlueprintAssignable)
	FOnQuakeLightPatternChangedSignature OnQuakeLightPatternChanged;

protected:

	/** If set this pattern will be used instead of the baseline Pattern */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quake Light")
	FString CustomPattern;

	/** The current output that has been generated */
	UPROPERTY(BlueprintReadOnly, Category = "Quake Light")
	float CurrentPatternOutput;

public:

	/**
	 * A rate of 1.0 runs at the rate it would in Quake and other Quake based engines.
	 * You can increase and decrease this value on the fly.
	 * Setting this value to 0.0 effectively pauses playback
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quake Light", meta = (UIMin = 0.0, ClampMin = 0.0))
	float PlaybackRate;

	/**
	 * Allows you to control the output amplitude of intensities set on the light component. 1.0 is default.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quake Light", meta = (UIMin = 0.0, ClampMin = 0.0))
	float Amplitude;

	// The accumulating playback time. You can set this explicitly for save / restore cases if needed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quake Light", meta = (UIMin = 0.0, ClampMin = 0.0), AdvancedDisplay)
	float PlaybackTime;

private:

	// The light component currently set
	UPROPERTY()
	class ULightComponent* LightComponent;

	// The string generated by CustomPattern
	TArray<ANSICHAR> CustomPatternString;

	// Set to what the lights original intensity was before being controlled
	float OriginalIntensity;

	// The actually used intensity (if overriden)
	float UsedIntensity;
};
