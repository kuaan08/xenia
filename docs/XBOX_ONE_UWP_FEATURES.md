# Xenia Canary UWP: Xbox One experimental features

This fork targets Xbox One Developer Mode (UWP, x64, Direct3D 12).
The ISO / XEX boot fix from commit b708758 is preserved. The changes
below are opt-in UI/CI features, not proof of full game compatibility.

## GitHub Actions build cache

The UWP workflow uses `actions/cache@v4` for:

* `.ci-cache/vulkan-sdk.exe`: downloaded Vulkan SDK installer (installation
  is still performed on a fresh runner).
* `build/`: CMake-generated MSVC project files, intermediate objects and
  static libraries.
* `xenia-canary-uwp/x64/Release/`: UWP frontend MSBuild intermediates.

Caches are keyed by Windows/CMake input identity and restored from a prior
successful job. On cold builds, cache misses or substantial source changes
may provide little or no speedup. Intermediate archives can be large;
monitor cache quota and retention in the repository's Actions settings.

## Interface language (Settings > General)

The new `ui_language` setting localizes the principal frontend headers,
settings categories, theme controls and experimental options. Current
choices: English (`en`), Brazilian Portuguese (`pt-BR`),
Spanish (`es`), French (`fr`), German (`de`), Italian (`it`).
English is the fallback for untranslated screens and text.

The existing `user_language` setting is a *separate* Xbox 360 guest
configuration. Changing interface language does not modify game locale.

## Theme presets (Settings > UI)

`ui_theme_preset` offers Classic Xbox 360, Xbox Green, Xbox Blue,
Xbox Purple, AMOLED Dark and Minimal Dark. These change UI color and text
effects and are saved in the configuration. AMOLED/Minimal use flat dark
backgrounds instead of loading the decorative frontend textures.
The existing custom background, accent, overlay and text effect settings
remain available for the classic/blade-based themes.

## Experimental options (Settings > Experimental)

The menu exposes existing engine CVARs:

| UI control | Underlying configuration | Default |
| --- | --- | --- |
| Asynchronous shaders | `async_shader_compilation` | true |
| D3D12 bindless resources | `d3d12_bindless` | true |
| Early D3D12 submission | `d3d12_submit_on_primary_buffer_end` | true |
| D3D12 page state tracking | `d3d12_clear_memory_page_state` | true |

* **Favor FPS: disable extra post-processing** selects bilinear image
  scaling and disables FXAA postprocessing. It does not change the game's
  actual internal rendering resolution or unlock frame rate.
* **Restore experimental defaults** resets the four checkboxes to their
  upstream defaults; it does **not** undo post-processing preferences.
* Restart the game after changing renderer or shader settings. Some flags
  may only take effect on initialization.
* Test per title; options may cause regressions or artifacts on the 2013
  Xbox One. No FPS improvement is promised.

## Smoke test

1. Build `Xenia Canary UWP x64 (experimental build)` in GitHub Actions.
2. Obtain the full signed installer artifact and certificate, install in
   Developer Mode and launch the frontend.
3. In **Paths**, verify that the existing ISO/XEX boot still succeeds.
4. Change **Settings > General > Interface Language**, return to the
   blades, and verify labels change without altering guest language.
5. Apply **Settings > UI > Theme Preset**; verify layout and background.
6. In **Settings > Experimental**, toggle one option at a time, restart the
   game and compare FPS, frame time, logs and graphical correctness.
7. Use **Restore experimental defaults** before filing regressions.

This is a research build; compatibility with games and full Xbox One
runtime behavior still requires device testing.
