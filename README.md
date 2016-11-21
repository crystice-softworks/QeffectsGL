# QeffectsGL
**Version 1.3 (November 2016)**

OpenGL pseudo-driver (wrapper) that adds some graphical enhancements to OpenGL-based games. You should place `opengl32.dll` and `QeffectsGL.ini` to the game executable's directory before running the game. If the game has multiple renderers available (e.g. Half-Life, Unreal), OpenGL renderer must be switched to.

## Setup

Please perform the following steps to install and enable QeffectsGL:

1. Place `opengl32.dll` and `QeffectsGL.ini` to the directory where the executable file of the game is located.
2. Run the game; make sure the `QeffectsGL.log` file is created; this means that game has successfully hooked the wrapper.
3. If you want to restore the native OpenGL renderer, delete the `opengl32.dll` file from the game executable's directory. You may keep `QeffectsGL.ini` for future use because if has no effect without the dll.

Refer to the **Customization** section for the list of tweakable parameters.
Please also read **Security Notice** before using QeffectsGL!

## Effects
- Anisotropic texture filtering
- Full-screen antialiasing (MSAA)
- HSL color correction
- Screen-space emboss bump-mapping
- Screen-space ambient occlusion
- Bloom
- Depth of Field (disabled by default)
- Vignette (disabled by default)

QeffectsGL also implements OpenGL extension limiting for some old games, such as Quake 2. Without such limit, old games can crash with modern video cards and drivers.

## Supported Games
- GLQuake
- Half-Life / CS 1.6 (Steam version may not work)
- Gunman Chronicles
- Quake2
- Kingpin
- Soldier of Fortune
- Unreal
- Unreal Tournament
- Quake3
- Return to Castle Wolfenstein
- Star Trek Voyager: Elite Force
- Tremulous
- American McGee's Alice
- Soldier of Fortune 2
- Doom 3
- Quake 4
However, it may also work with other OpenGL games. Just try!

## Gallery
Left image: without QeffectsGL; right image: with QeffectsGL.


## Customization

By default, QeffectsGL takes parameters from the `[Generic]` section of the `QeffectsGL.ini` configuration file. They can be overriden by a section named after the game's executable, e.g. `[quake2.exe]`. The following parameters can be customized:

| Parameter        | Default value           | Meaning  |
| ------------- |:-------------:| ----- |
| RenderFrame | -1 | The most important command, selects an internal marker when QeffectsGL's effects should be applied; set to -1 to apply and the very end of the frame, this works almost always, but the effects will also be applied to the HUD and stuff. Try to experiment with values 0, 1, 2, 3... to find a proper injection point. However sometimes this is not possible and/or stable, so -1 is your final choice. |
| Anisotropy | 8 | Anisotropic texture filtering, set to zero to use game settings. Some old games don't support anisotropic filtering, so the default is 8. |
| Multisample | 4  | MSAA anti-aliasing. Some old games don't support MSAA, so the default is 4. |
| Bloom | 1 | Enable bloom effect. |
| BloomSteps | 8 | Number of bloom blurring passes. Higher values look better but are slower. |
| BloomDarken | 3 | How much the source scene is darkened before bloom blurring. Brigher games require higher values to reduce blooming on regular stuff. |
| BloomScale | 1.5 | Final bloom effect scale. Reduce this value if everything looks too bright. |
| SSAO | 1 | Enable screen-space ambient occlusion effect. |
| SSAOScale | 8.0 | SSAO effect scale, higher values produce more noticable occlusion effects. |
| SSAOMinZ | 0.005 | Minimum scene depth to apply SSAO effect. Can be tweaked to remove SSAO on first-person guns, etc. |
| SSAOMaxZ | 1.0 | Maximum scene depth to apply SSAO effect. Can be tweaked to remove SSAO on far objects, e.g. sky. |
| Hue | 0 | HSL color correction: hue. Same as in Adobe Photoshop. |
| Saturation | 0 | HSL color correction: saturation. Same as in Adobe Photoshop. |
| Lightness | 0 | HSL color correction: lightness. Same as in Adobe Photoshop. |
| Emboss | 1 | Enable screen-space emboss bump-mapping effect. |
| EmbossScale | 0.5 | Emboss effect scale; higher values produce darker emboss effect. |
| DOF | 1 | Enable depth-of-field effect. |
| DOFSpeed | 1.0 | Speed at which focus will be approached. |
| DOFScale | 2.0 | DOF effect scale; higher values make background not in focus more blurred. |
| Vignette | 1 | Enable vignette effect (screen is darker at corners). |
| VignetteScale | 1.25 | Vignette scale, 0.0-2.0; higher values make corners darker. |
| ExtensionLimit | 0 | 0 means no limit; if you set it to say 4000, only those extensions that not totally exceed 4000 characters will be reported to the game; the exception is WGL_EXT_swap_control which is always reported (unless limit is less that its length). |
| ZTrickFix | 0 | Some old games use a so-called *z-trick* to eliminate depth buffer clearing; QeffectsGL is not compatible with z-trick. If you suspect the game uses it, set this to 1, and QeffectsGL will attempt to get rid of the z-trick behavoir. |
| ViewportFix | 0 | Some games (e.g. `GLQuake`) don't render to the full screen, and don't like QeffectsGL to maximize the viewport when applying effects. Set this to 1, and QeffectsGL will use the viewport at the injection point unmodified. |
| CompareMode | 0 | Set to 1 if you want to compare the scene with and without QeffectsGL; this turns QeffectsGL on and off every second. Not for a playable releases! |

**Note:** Default values in the table correspond to dll's internal defaults; some of them are already overriden in the `[Generic]` section of the `QeffectsGL.ini`.

## Troubleshooting

Sometimes QeffectsGL won't be hooked by the game. There are several workarounds possible:
- First of all, make sure that this is an OpenGL-based game; ask developers or search the Web to be sure.
- NVIDIA drivers sometimes use profiles for the games; they pre-load system's OpenGL library before the game loads it, and the wrapper is ignored. To deal with this, try to rename the executable: e.g. rename `quake2.exe` to `quake2_.exe`, and use it to run the game. Please also rename the corresponding entry in the `QeffectsGL.ini` file for the game!

## Security Notice

Some anti-cheating software refers such wrappers to as cheats, so make sure your anti-cheating software is not active. I.e. don't forget to delete wrapper's `opengl32.dll` before playing multiplayer games: you may get a ban because of Valve Anti-Cheat, and it is nearly impossible to reverse. So, the rule of thumb is: **don't use QeffectsGL with multiplayer Steam games**.

**NEVER**, you hear, **NEVER** replace system's OpenGL library `opengl32.dll`! This can break the whole enchilada, since QeffectsGL is NOT a complete replacement for OpenGL.

*Crystice Softworks*
*2016*
