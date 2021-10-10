// Copyright 2020-2021 Solar Storm Interactive. All Rights Reserved.

#include "QuakeLightPatternsComponent.h"
#include "Components/LightComponent.h"

// The baseline patterns used.
static const ANSICHAR* BaselineLightPatterns[] =
{
	"m",
	"mmnmmommommnonmmonqnmmo",
	"abcdefghijklmnopqrstuvwxyzyxwvutsrqponmlkjihgfedcba",
	"mmmmmaaaaammmmmaaaaaabcdefgabcdefg",
	"mamamamamama",
	"jklmnopqrstuvwxyzyxwvutsrqponmlkj",
	"nmonqnmomnmomomno",
	"mmmaaaabcdefgmmmmaaaammmaamm",
	"mmmaaammmaaammmabcdefaaaammmmabcdefmmmaaaa",
	"aaaaaaaazzzzzzzz",
	"mmamammmmammamamaaamammma",
	"abcdefghijklmnopqrrqponmlkjihgfedcba",
	"mmnnmmnnnmmnn",
};
static_assert(sizeof(BaselineLightPatterns) / sizeof(intptr_t) == static_cast<size_t>(EQuakeLightPattern::LAST_PATTERN_INDEX), "Baseline patterns and light patterns enum doesn't align!");

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UQuakeLightPatternsComponent::UQuakeLightPatternsComponent()
	: Pattern(EQuakeLightPattern::Normal)
	, CurrentPatternOutput(0.0f)
	, PlaybackRate(1.0f)
	, Amplitude(1.0f)
	, PlaybackTime(0.0f)
	, LightComponent(nullptr)
	, OriginalIntensity(1.0f)
	, UsedIntensity(1.0f)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.TickInterval = 0.0f;
	bAutoActivate = false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void UQuakeLightPatternsComponent::BeginPlay()
{
	Super::BeginPlay();

	if(!CustomPattern.IsEmpty())
	{
		SetCustomPattern(CustomPattern);
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void UQuakeLightPatternsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Increase the playback time
	PlaybackTime += PlaybackRate * DeltaTime;

	// The algorithm here follows closely to the original in quake (with naming changes mostly and feature extensions)
	// See https://github.com/id-Software/Quake/blob/bf4ac424ce754894ac8f1dae6a3981954bc9852d/WinQuake/gl_rlight.c#L27-L52
	const int32 timeIndex = static_cast<int32>(PlaybackTime * 10.0f);

	const ANSICHAR* pattern = nullptr;
	if(CustomPatternString.Num())
	{
		pattern = CustomPatternString.GetData();
	}
	else if(Pattern >= EQuakeLightPattern::Normal && Pattern < EQuakeLightPattern::LAST_PATTERN_INDEX)
	{
		pattern = BaselineLightPatterns[static_cast<size_t>(Pattern)];
	}

	if(!pattern)
	{
		return;
	}

	const int32 charIndex = timeIndex % FCStringAnsi::Strlen(pattern);
	const int32 lightValue = (pattern[charIndex] - static_cast<ANSICHAR>('a')) * 22;

	CurrentPatternOutput = lightValue / 264.0f;

	if(LightComponent)
	{
		LightComponent->SetIntensity(UsedIntensity * CurrentPatternOutput * Amplitude);
	}

	OnQuakeLightPatternChanged.Broadcast(pattern[charIndex], CurrentPatternOutput);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void UQuakeLightPatternsComponent::Activate(bool bReset)
{
	Super::Activate(bReset);

	if(!LightComponent)
	{
		AActor* owner = GetOwner();
		if(owner)
		{
			LightComponent = owner->FindComponentByClass<ULightComponent>();
			if(LightComponent)
			{
				OriginalIntensity = UsedIntensity = LightComponent->Intensity;
			}
		}
	}
	else
	{
		OriginalIntensity = UsedIntensity = LightComponent->Intensity;
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void UQuakeLightPatternsComponent::Deactivate()
{
	Super::Deactivate();

	if(LightComponent)
	{
		LightComponent->SetIntensity(OriginalIntensity);
	}

	PlaybackTime = 0.0f;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void UQuakeLightPatternsComponent::SetLightComponent(ULightComponent* lightComponent, const float overrideBaseIntensity)
{
	if(lightComponent != LightComponent)
	{
		if(LightComponent)
		{
			LightComponent->SetIntensity(OriginalIntensity);
			LightComponent = nullptr;
		}

		PlaybackTime = 0.0f;

		if(lightComponent)
		{
			LightComponent = lightComponent;
			OriginalIntensity = LightComponent->Intensity;
			if(overrideBaseIntensity >= 0.0f)
			{
				UsedIntensity = overrideBaseIntensity;
			}
			else
			{
				UsedIntensity = OriginalIntensity;
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void UQuakeLightPatternsComponent::SetCustomPattern(const FString& pattern)
{
	if(pattern.IsEmpty())
	{
		// clear out the custom pattern
		CustomPatternString.Empty();
		return;
	}

	CustomPattern = pattern;
	
	const ANSICHAR* patternChars = TCHAR_TO_ANSI(*pattern);
	CustomPatternString.AddDefaulted(FCStringAnsi::Strlen(patternChars) + 1);

	for(int32 cIndex = 0; cIndex < FCStringAnsi::Strlen(patternChars); ++cIndex)
	{
		// We clamp the pattern down to reasonable values
		CustomPatternString[cIndex] = FMath::Clamp(patternChars[cIndex], static_cast<ANSICHAR>('a'), static_cast<ANSICHAR>('z'));
	}

	// Need to put on a null on explicitly.
	CustomPatternString[CustomPatternString.Num() - 1] = static_cast<ANSICHAR>('\0');
}
