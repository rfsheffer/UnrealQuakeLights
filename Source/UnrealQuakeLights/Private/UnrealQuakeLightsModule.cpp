// Copyright 2020-2021 Solar Storm Interactive. All Rights Reserved.

#include "UnrealQuakeLightsModule.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "UnrealQuakeLights"

class FUnrealQuakeLightsModule : public IUnrealQuakeLightsModule
{
public:
    FUnrealQuakeLightsModule()
    {
    }

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_MODULE(FUnrealQuakeLightsModule, UnrealQuakeLights);

//////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------------------------------------
/**
*/
void FUnrealQuakeLightsModule::StartupModule()
{
}

//--------------------------------------------------------------------------------------------------------------------
/**
*/
void FUnrealQuakeLightsModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
