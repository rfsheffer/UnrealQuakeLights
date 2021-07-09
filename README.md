# Unreal Quake Lights
A component that outputs light intensities to a specified light component in patterns originally defined in the 1996 game Quake.

These light patterns were used in Quake and offshoot engines such as GoldSrc and the SourceEngine and most recently in the game Half-Life Alyx.

This is an implementation of the playback of these light patterns overtime for the Unreal Engine.

## Basic How To Use
- Assign the 'QuakeLightPatternsComponent' component to your actor
  - Setup the properties on the component to your liking (choose a baseline pattern from Quake or define your own)
- Add a light component to your actor
  - Set the base intensity for the light, this will animate against the pattern set in 'QuakeLightPatternsComponent'
- Activate the 'QuakeLightPatternsComponent' component or set it to auto active

You can also set the light you want animated explicitly by calling 'SetLightComponent'

![Alt text](/Screenshots/Fluorflick.gif?raw=true "Quake Fluorescent Flicker light pattern")

Copyright 2020-2021 Solar Storm Interactive. All Rights Reserved.

MIT Licensed