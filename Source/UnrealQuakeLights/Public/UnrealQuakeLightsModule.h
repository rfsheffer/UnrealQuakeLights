// Copyright 2020-2021 Solar Storm Interactive. All Rights Reserved.

#pragma once

#include "Engine.h"

#define UNREAL_QUAKE_LIGHTS_MODULE_NAME "UnrealQuakeLights"

//////////////////////////////////////////////////////////////////////////
// INextLifeModule

class IUnrealQuakeLightsModule : public IModuleInterface
{
public:
    /**
    * Singleton-like access to this module's interface.  This is just for convenience!
    * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
    *
    * @return Returns singleton instance, loading the module on demand if needed
    */
    static FORCEINLINE IUnrealQuakeLightsModule& Get()
    {
        return FModuleManager::LoadModuleChecked< IUnrealQuakeLightsModule >(UNREAL_QUAKE_LIGHTS_MODULE_NAME);
    }

    /**
    * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
    *
    * @return True if the module is loaded and ready to use
    */
    static FORCEINLINE bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded(UNREAL_QUAKE_LIGHTS_MODULE_NAME);
    }
};
