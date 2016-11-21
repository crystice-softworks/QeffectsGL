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

**GLQuake**

[![q11a](https://cloud.githubusercontent.com/assets/20521208/20474316/912dacd6-aff8-11e6-90e7-aa3cb8b9c8a8.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474495/50a14758-aff9-11e6-8014-8c60a6a7fa8e.jpg)
[![q11b](https://cloud.githubusercontent.com/assets/20521208/20474317/912eb176-aff8-11e6-867a-9f7ba5638166.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474494/50a11bac-aff9-11e6-83dd-98d081191b81.jpg)

[![q12a](https://cloud.githubusercontent.com/assets/20521208/20474319/912f591e-aff8-11e6-916d-64897df8957a.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474496/50a1c886-aff9-11e6-8b85-2d00c53b2dba.jpg)
[![q12b](https://cloud.githubusercontent.com/assets/20521208/20474318/912f429e-aff8-11e6-9a26-6cf409d59d57.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474497/50a36330-aff9-11e6-9370-ff06ee2469ba.jpg)

**Half-Life**

[![hl1a](https://cloud.githubusercontent.com/assets/20521208/20474307/90e9063a-aff8-11e6-9073-f525a733f92e.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474484/505cbe4e-aff9-11e6-9866-933c84d45e94.jpg)
[![hl1b](https://cloud.githubusercontent.com/assets/20521208/20474309/90ea2dbc-aff8-11e6-9884-0afb4f8aeec7.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474488/507f063e-aff9-11e6-8017-b630537980f4.jpg)

[![hl2a](https://cloud.githubusercontent.com/assets/20521208/20474310/910ab00a-aff8-11e6-8823-1743d7cfb107.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474487/507eaeb4-aff9-11e6-8ee4-a3b087ad5c4e.jpg)
[![hl2b](https://cloud.githubusercontent.com/assets/20521208/20474313/910c7c82-aff8-11e6-9b57-ea290640e63e.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474489/507efafe-aff9-11e6-85f9-3d9386287b22.jpg)

**Counter-Strike 1.6**

[![cs161a](https://cloud.githubusercontent.com/assets/20521208/20474296/90a316d4-aff8-11e6-9e24-03a6fe38fa8a.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474471/5010fd88-aff9-11e6-8cb9-9b88428b364c.jpg)
[![cs161b](https://cloud.githubusercontent.com/assets/20521208/20474297/90a3f6e4-aff8-11e6-930a-2054d4dd198d.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474477/50360fce-aff9-11e6-905d-9a3cbc372b18.jpg)

[![cs162a](https://cloud.githubusercontent.com/assets/20521208/20474300/90c57e18-aff8-11e6-8505-65092a400a22.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474476/5035b286-aff9-11e6-9ff5-8a613e4bd253.jpg)
[![cs162b](https://cloud.githubusercontent.com/assets/20521208/20474301/90c5e6f0-aff8-11e6-83bf-50fa002f0168.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474480/503936d6-aff9-11e6-9946-371fd1b9bf94.jpg)

**Gunman Chronicles**

[![gunman1a](https://cloud.githubusercontent.com/assets/20521208/20474304/90e80654-aff8-11e6-8dfe-4065c758606b.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474482/505b5392-aff9-11e6-9a86-702afb8a82bc.jpg)
[![gunman1b](https://cloud.githubusercontent.com/assets/20521208/20474306/90e89380-aff8-11e6-81c6-6da22b53c6d5.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474485/505d666e-aff9-11e6-8b5c-0d36c09575a6.jpg)

[![gunman2a](https://cloud.githubusercontent.com/assets/20521208/20474305/90e8a622-aff8-11e6-86e9-b80c590f3bb7.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474486/507c2306-aff9-11e6-825f-0bc2240833d1.jpg)
[![gunman2b](https://cloud.githubusercontent.com/assets/20521208/20474308/90e908b0-aff8-11e6-8d79-d68f54a6e6f6.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474483/505bfb9e-aff9-11e6-82c8-769328a7df9e.jpg)

**Quake 2**

[![q21a](https://cloud.githubusercontent.com/assets/20521208/20474320/91309f0e-aff8-11e6-9041-cfbeeb37beae.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474498/50c10e12-aff9-11e6-8bc4-338535a92131.jpg)
[![q21b](https://cloud.githubusercontent.com/assets/20521208/20474321/9130a418-aff8-11e6-8c86-fdbb3e75ffcc.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474499/50c28544-aff9-11e6-8310-27f8d2ef33b4.jpg)

[![q22a](https://cloud.githubusercontent.com/assets/20521208/20474323/91525f04-aff8-11e6-81c0-68e9a006e9f5.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474500/50c2d42c-aff9-11e6-915b-39dc30597af7.jpg)
[![q22b](https://cloud.githubusercontent.com/assets/20521208/20474324/91527d0e-aff8-11e6-944e-f6febedf739e.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474502/50c40a54-aff9-11e6-9137-084c458b7651.jpg)

**Kingpin**

[![kingpin1a](https://cloud.githubusercontent.com/assets/20521208/20474311/910c0888-aff8-11e6-9be0-b792da6fc765.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474490/5080e03a-aff9-11e6-8421-0439d45a6456.jpg)
[![kingpin1b](https://cloud.githubusercontent.com/assets/20521208/20474314/910cdf92-aff8-11e6-8142-5ace2476435e.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474491/5081991c-aff9-11e6-997f-ab09fd52c7a1.jpg)

[![kingpin2a](https://cloud.githubusercontent.com/assets/20521208/20474312/910c5e64-aff8-11e6-8bf7-c23cfa643412.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474492/509e9c92-aff9-11e6-9d50-11b69f2ad275.jpg)
[![kingpin2b](https://cloud.githubusercontent.com/assets/20521208/20474315/910d338e-aff8-11e6-96e1-d8129ffe690d.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474493/50a06ce8-aff9-11e6-882b-6b23269e6ef1.jpg)

**Soldier of Fortune**

[![sof1a](https://cloud.githubusercontent.com/assets/20521208/20474332/9176b0ac-aff8-11e6-82bd-77b936e3a43d.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474510/51062ad8-aff9-11e6-8924-040175d18339.jpg)
[![sof1b](https://cloud.githubusercontent.com/assets/20521208/20474333/91780c0e-aff8-11e6-9f8c-f53d4c6af526.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474511/5106a2ba-aff9-11e6-9f41-61f953070885.jpg)

[![sof2a](https://cloud.githubusercontent.com/assets/20521208/20474335/9197d246-aff8-11e6-8b5d-25d8eec57f09.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474512/5107fc6e-aff9-11e6-967b-5fef6e431ad4.jpg)
[![sof2b](https://cloud.githubusercontent.com/assets/20521208/20474340/919f223a-aff8-11e6-8a57-e7e04af2bf78.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474513/5108b136-aff9-11e6-8316-29df616cf25e.jpg)

**Quake 3**

[![q31a](https://cloud.githubusercontent.com/assets/20521208/20474325/91529136-aff8-11e6-8f28-145fef2f9598.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474501/50c3abea-aff9-11e6-9e8d-f5889cf2f223.jpg)
[![q31b](https://cloud.githubusercontent.com/assets/20521208/20474322/9152359c-aff8-11e6-86cf-58208b014968.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474503/50c5060c-aff9-11e6-8614-9b837ae39625.jpg)

[![q32a](https://cloud.githubusercontent.com/assets/20521208/20474326/9153b336-aff8-11e6-8d4c-a25b3ceeb339.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474504/50e39770-aff9-11e6-8272-a17527a3fa39.jpg)
[![q32b](https://cloud.githubusercontent.com/assets/20521208/20474327/915426a4-aff8-11e6-84bf-924c7f0caf22.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474505/50e54d0e-aff9-11e6-9e8c-46fcc2fa7fed.jpg)

[![sq3t11a](https://cloud.githubusercontent.com/assets/20521208/20474341/91bbbe36-aff8-11e6-91f2-0c3c30d0d82f.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474518/5129b516-aff9-11e6-9174-ff227b212a51.jpg)
[![sq3t11b](https://cloud.githubusercontent.com/assets/20521208/20474342/91bc4b30-aff8-11e6-9d02-5c1e71428b3d.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474519/5129fd14-aff9-11e6-9790-731948d6f49b.jpg)

[![sq3t22a](https://cloud.githubusercontent.com/assets/20521208/20474343/91bdbde4-aff8-11e6-891d-f84087c90cc6.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474520/512b0e8e-aff9-11e6-9e6b-837fdf78bfbb.jpg)
[![sq3t22b](https://cloud.githubusercontent.com/assets/20521208/20474344/91be77ca-aff8-11e6-81da-32ed8ab311a5.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474521/512b6b2c-aff9-11e6-93fd-3e85ca2d9e90.jpg)

**Return to Castle Wolfenstein**

[![rtcw1a](https://cloud.githubusercontent.com/assets/20521208/20474328/9174b7ca-aff8-11e6-975f-b48a11c0b852.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474506/50e562e4-aff9-11e6-8e75-b392bd556028.jpg)
[![rtcw1b](https://cloud.githubusercontent.com/assets/20521208/20474331/91768348-aff8-11e6-8aa1-213ce1d10288.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474509/50e78290-aff9-11e6-8279-b335bbee50b6.jpg)

[![rtcw2a](https://cloud.githubusercontent.com/assets/20521208/20474330/91764d7e-aff8-11e6-8b69-2cbab27a2bd2.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474507/50e662f2-aff9-11e6-9988-3408dfc3dc64.jpg)
[![rtcw2b](https://cloud.githubusercontent.com/assets/20521208/20474329/917552ac-aff8-11e6-88c0-f212a3164d3b.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474508/50e7577a-aff9-11e6-96a1-37af20178686.jpg)

**American McGee's Alice**

[![alice1a](https://cloud.githubusercontent.com/assets/20521208/20474292/90a0d31a-aff8-11e6-96f3-9aae7694bef4.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474470/5010f040-aff9-11e6-8319-e4de630506f1.jpg)
[![alice1b](https://cloud.githubusercontent.com/assets/20521208/20474294/90a1fc22-aff8-11e6-9fce-bd7c7180e4f3.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474469/50109d52-aff9-11e6-8aa7-37585b7251e7.jpg)

[![alice2a](https://cloud.githubusercontent.com/assets/20521208/20474295/90a24196-aff8-11e6-9b77-d485ffaf7909.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474472/50110922-aff9-11e6-9f8b-d04a6dbd2901.jpg)
[![alice2b](https://cloud.githubusercontent.com/assets/20521208/20474293/90a1f164-aff8-11e6-8d12-ac9e7bbfbbb4.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474474/50113884-aff9-11e6-98de-5f04d7858b2d.jpg)

**Soldier of Fortune 2**

[![sof21a](https://cloud.githubusercontent.com/assets/20521208/20474337/919b9804-aff8-11e6-8efa-d65997d71cd7.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474514/5109ed80-aff9-11e6-9487-e0a2064f80ff.jpg)
[![sof21b](https://cloud.githubusercontent.com/assets/20521208/20474336/91995df0-aff8-11e6-8ce2-93a8cfcbc627.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474515/510a973a-aff9-11e6-9db6-63c8bfa4a490.jpg)

[![sof22a](https://cloud.githubusercontent.com/assets/20521208/20474338/919bf632-aff8-11e6-9075-59bed43458dc.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474516/512902b0-aff9-11e6-8044-c61a0db2daaa.jpg)
[![sof22b](https://cloud.githubusercontent.com/assets/20521208/20474339/919cabe0-aff8-11e6-8dd3-83c085d138d7.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474517/5129996e-aff9-11e6-838f-54c3be646974.jpg)

**Star Trek Voyager: Elite Force**

[![stvef1a](https://cloud.githubusercontent.com/assets/20521208/20474345/91c1b584-aff8-11e6-9428-3c8344727166.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474522/514c001c-aff9-11e6-8d3b-abb53457bb97.jpg)
[![stvef1b](https://cloud.githubusercontent.com/assets/20521208/20474346/91c1fbca-aff8-11e6-88ef-1afe9cf19048.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474523/514cf724-aff9-11e6-86f5-4edea3a8b78f.jpg)

[![stvef2a](https://cloud.githubusercontent.com/assets/20521208/20474347/9212f070-aff8-11e6-9521-585b2e0f37e6.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474527/514eae20-aff9-11e6-9585-5c8fcb2f99ec.jpg)
[![stvef2b](https://cloud.githubusercontent.com/assets/20521208/20474348/921415c2-aff8-11e6-9aac-7ad54f7576be.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474524/514e3f6c-aff9-11e6-9f9a-46fbb10096f2.jpg)

**Tremulous**

[![tremulous1a](https://cloud.githubusercontent.com/assets/20521208/20474351/9214d6ec-aff8-11e6-941c-4aac740ed31b.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474526/514ea2ea-aff9-11e6-88f2-fd4d0b35495f.jpg?raw=true)
[![tremulous1b](https://cloud.githubusercontent.com/assets/20521208/20474349/9214166c-aff8-11e6-9b57-4802496d4d58.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474525/514e7edc-aff9-11e6-9008-d1338950661e.jpg?raw=true)

[![tremulous2a](https://cloud.githubusercontent.com/assets/20521208/20474350/9214894e-aff8-11e6-9e2b-887e5724052a.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474528/516df2e4-aff9-11e6-8f75-6cc6f1608cda.jpg?raw=true)
[![tremulous2b](https://cloud.githubusercontent.com/assets/20521208/20474291/9082c33e-aff8-11e6-952b-cfbaf793db38.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474473/50111b74-aff9-11e6-96bc-8d21f0407b7a.jpg?raw=true)

**Doom 3**

[![d31a](https://cloud.githubusercontent.com/assets/20521208/20474299/90c56d7e-aff8-11e6-9537-e02fc42a344f.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474475/5035ada4-aff9-11e6-93c8-2df12a22f770.jpg?raw=true)
[![d31b](https://cloud.githubusercontent.com/assets/20521208/20474298/90c508a2-aff8-11e6-9676-9c85535950cf.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474479/503653f8-aff9-11e6-910e-67521ec960f1.jpg?raw=true)

[![d32a](https://cloud.githubusercontent.com/assets/20521208/20474302/90c64dde-aff8-11e6-8cc6-9e87345479e9.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474478/503627de-aff9-11e6-94c0-0bb864b63b65.jpg?raw=true)
[![d32b](https://cloud.githubusercontent.com/assets/20521208/20474303/90c844ae-aff8-11e6-8a39-dd20bea59e79.jpg?raw=true)](https://cloud.githubusercontent.com/assets/20521208/20474481/505b381c-aff9-11e6-9cc1-698f4acf2d44.jpg?raw=true)

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