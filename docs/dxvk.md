# DXVK in the mod launcher

C17VR renders through [DXVK](https://github.com/doitsujin/dxvk) (Direct3D 9 on Vulkan) on Windows. The engine, `materialsystem.dll` and `shaderapidx9.dll` are closed binaries from Source SDK Base 2013 MP, so DXVK can't be compiled into them. Instead the mod launcher loads DXVK's `d3d9.dll` before the engine starts.

Vulkan is also what the SteamVR render path needs: DXVK's D3D9/Vulkan interop exposes the eye render targets as Vulkan images the compositor can take.

## How it works

`LoadBundledDXVK()` in `src/launcher_main/main.cpp` runs in `WinMain` right before `launcher.dll` is loaded:

1. It looks for `<game>\bin\x64\dxvk\d3d9.dll`, where `<game>` is the folder holding the mod's `*_win64.exe` (the same `bin\x64` that holds `steam_api64.dll`).
2. It checks that the system has a Vulkan loader (`vulkan-1.dll` in System32).
3. If `dxvk.conf` sits next to the DLL and `DXVK_CONFIG_FILE` isn't already set, it points `DXVK_CONFIG_FILE` at it.
4. It loads the DLL by full path. When `shaderapidx9.dll` later imports `d3d9.dll` by name, Windows returns the module that's already loaded instead of the one in System32.

Each outcome is written with `OutputDebugString`, prefixed `[C17VR]`, so you can see it in the Visual Studio output window or in DebugView.

## Getting and shipping the DLL

The DLL isn't committed to the repo (`bin/` is git-ignored). To fetch it:

```
python src/devtools/fetch_dxvk.py
```

That downloads the pinned DXVK release (`DXVK_VERSION` in the script) from GitHub and writes:

```
game/bin/x64/dxvk/d3d9.dll   DXVK's 64-bit D3D9 implementation
game/bin/x64/dxvk/DXVK.txt   version, source URL, tarball SHA-256, license note
```

Use `--version` to pick another release, `--archive` to use a tarball you've already downloaded, and `--sha256` to check the tarball against a known hash. After the first fetch, put the printed hash into your release process so later fetches can be verified.

To ship the mod, include the whole `bin\x64\dxvk\` folder next to `bin\x64\steam_api64.dll`. You can add a `dxvk.conf` there for mod-wide DXVK settings. DXVK is zlib-licensed; keep `DXVK.txt` (or an equivalent notice) in what you ship.

Only the 64-bit build is handled. The 32-bit launcher relaunches into the `_win64.exe` before it gets this far.

## Falling back to native Direct3D 9

The launcher uses Windows' own D3D9 when any of these is true:

- The game is launched with `-nodxvk` (Steam: Properties > Launch Options).
- `bin\x64\dxvk\d3d9.dll` isn't there. Deleting the folder turns DXVK off for good.
- There's no Vulkan loader on the system.
- The DLL fails to load.

A GPU that has a Vulkan loader but can't run DXVK (it needs Vulkan 1.3) isn't caught by these checks. The engine will fail to create its device. Players with such hardware should use `-nodxvk`.

## Linux

Nothing changes on Linux. The launcher `exec`s the SDK's `hl2.sh`, and the 2025 SDK already renders through DXVK-native there. That last part is inferred from Valve's 64-bit TF2 update and hasn't been checked in this repo.
