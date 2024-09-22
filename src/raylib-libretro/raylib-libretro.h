/**********************************************************************************************
 *
 *   rlibretro - Raylib extension to interact with libretro cores.
 *
 *   DEPENDENCIES:
 *            - raylib
 *            - dl
 *            - libretro-common
 *              - dynamic/dylib
 *              - libretro.h
 *
 *   LICENSE: zlib/libpng
 *
 *   rLibretro is licensed under an unmodified zlib/libpng license, which is an
 *OSI-certified, BSD-like license that allows linking with closed source
 *software:
 *
 *   Copyright (c) 2020 Rob Loach (@RobLoach)
 *
 *   This software is provided "as-is", without any express or implied warranty.
 *In no event will the authors be held liable for any damages arising from the
 *use of this software.
 *
 *   Permission is granted to anyone to use this software for any purpose,
 *including commercial applications, and to alter it and redistribute it freely,
 *subject to the following restrictions:
 *
 *     1. The origin of this software must not be misrepresented; you must not
 *claim that you wrote the original software. If you use this software in a
 *product, an acknowledgment in the product documentation would be appreciated
 *but is not required.
 *
 *     2. Altered source versions must be plainly marked as such, and must not
 *be misrepresented as being the original software.
 *
 *     3. This notice may not be removed or altered from any source
 *distribution.
 *
 **********************************************************************************************/

#ifndef RAYLIB_LIBRETRO_RLIBRETRO_H
#define RAYLIB_LIBRETRO_RLIBRETRO_H

#include "libretro.h"
#include "raylib.h"

#if defined(__cplusplus)
extern "C" { // Prevents name mangling of functions
#endif

//------------------------------------------------------------------------------------
// Libretro Raylib Integration (Module: rlibretro)
//------------------------------------------------------------------------------------

bool InitLibretro(const char *core); // Initialize the given libretro core.
bool LoadLibretroGame(
    const char *gameFile);  // Load the provided content. Provide NULL to load
                            // the core without content.
bool IsLibretroReady();     // Whether or not the core was successfully loaded.
bool IsLibretroGameReady(); // Whether or not the game has been loaded.
void UpdateLibretro();      // Run an iteration of the core.
bool LibretroShouldClose(); // Check whether or not the core has
                            // requested to shutdown.
void DrawLibretro();        // Draw the libretro state on the screen.
void DrawLibretroTint(
    Color tint); // Draw the libretro state on the screen with a tint.
void DrawLibretroEx(Vector2 position, float rotation, float scale, Color tint);
void DrawLibretroV(Vector2 position, Color tint);
void DrawLibretroTexture(int posX, int posY, Color tint);
void DrawLibretroPro(Rectangle destRec, Color tint);
const char *GetLibretroName(); // Get the name of the loaded libretro core.
unsigned GetLibretroWidth();   // Get the desired width of the libretro core.
unsigned GetLibretroHeight();  // Get the desired height of the libretro core.
Texture2D
GetLibretroTexture(); // Retrieve the texture used to render the libretro state.
bool DoesLibretroCoreNeedContent(); // Determine whether or not the
                                    // loaded core require content.
void UnloadLibretroGame();          // Unload the game that's currently loaded.
void CloseLibretro();               // Close the initialized libretro core.

void LibretroMapPixelFormatARGB1555ToRGB565(void *output_, const void *input_,
                                            int width, int height,
                                            int out_stride, int in_stride);
void LibretroMapPixelFormatARGB8888ToARGB8888(void *output_, const void *input_,
                                              int width, int height,
                                              int out_stride, int in_stride);

void conv_argb8888_rgba4444(void *output_, const void *input_, int width,
                            int height, int out_stride, int in_stride);

int LibretroMapRetroPixelFormatToPixelFormat(int pixelFormat);
int LibretroMapRetroJoypadButtonToGamepadButton(int button);
int LibretroMapRetroJoypadButtonToRetroKey(int button);
int LibretroMapRetroKeyToKeyboardKey(int key);
int LibretroMapRetroLogLevelToTraceLogType(int level);

// the knobend who wrote this library gave a bin src that imports (EVERYTHING
// ELSE FROM THE .C FILE THAT ISN'T IN THE .H FILE! LOL??)

typedef struct rLibretro {
  // Dynamic library symbols.
  void *handle;
  void (*retro_init)(void);
  void (*retro_deinit)(void);
  unsigned (*retro_api_version)(void);
  void (*retro_set_environment)(retro_environment_t);
  void (*retro_set_video_refresh)(retro_video_refresh_t);
  void (*retro_set_audio_sample)(retro_audio_sample_t);
  void (*retro_set_audio_sample_batch)(retro_audio_sample_batch_t);
  void (*retro_set_input_poll)(retro_input_poll_t);
  void (*retro_set_input_state)(retro_input_state_t);
  void (*retro_get_system_info)(struct retro_system_info *);
  void (*retro_get_system_av_info)(struct retro_system_av_info *);
  void (*retro_set_controller_port_device)(unsigned port, unsigned device);
  void (*retro_reset)(void);
  void (*retro_run)(void);
  size_t (*retro_serialize_size)(void);
  bool (*retro_serialize)(void *, size_t);
  bool (*retro_unserialize)(const void *, size_t);
  void (*retro_cheat_reset)(void);
  void (*retro_cheat_set)(unsigned index, bool enabled, const char *code);
  bool (*retro_load_game)(const struct retro_game_info *);
  bool (*retro_load_game_special)(unsigned, const struct retro_game_info *,
                                  size_t);
  void (*retro_unload_game)(void);
  unsigned (*retro_get_region)(void);
  void *(*retro_get_memory_data)(unsigned);
  size_t (*retro_get_memory_size)(unsigned);

  // System Information.
  bool shutdown;
  unsigned width, height, fps;
  double sampleRate;
  float aspectRatio;
  char libraryName[200];
  char libraryVersion[200];
  char validExtensions[200];
  bool needFullpath;
  bool blockExtract;
  bool supportNoGame;
  unsigned apiVersion;
  enum retro_pixel_format pixelFormat;
  unsigned performanceLevel;
  bool loaded;
  float volume;
  // The last performance counter registered. TODO: Make it a linked list.
  struct retro_perf_counter *perf_counter_last;
  struct retro_frame_time_callback runloop_frame_time;
  retro_usec_t runloop_frame_time_last;
  struct retro_audio_callback audio_callback;

  // Game data.
  Vector2 inputLastMousePosition;
  Vector2 inputMousePosition;

  // Raylib objects used to play the libretro core.
  Texture texture;

  // Audio
  AudioStream audioStream;
  int16_t *audioBuffer;
  size_t audioFrames;
} rLibretro;

rLibretro *GetLibretroCore();
void *GetSystemMemory();
void LibretroLogger(enum retro_log_level level, const char *fmt, ...);
void LibretroInitVideo();
retro_time_t LibretroGetTimeUSEC();
uint64_t LibretroGetCPUFeatures();
retro_perf_tick_t LibretroGetPerfCounter();
void LibretroPerfRegister(struct retro_perf_counter *counter);
void LibretroPerfStart(struct retro_perf_counter *counter);
void LibretroPerfStop(struct retro_perf_counter *counter);
void LibretroPerfLog();
bool LibretroSetEnvironment(unsigned cmd, void *data);
bool LibretroGetAudioVideo();
void UpdateLibretro();
bool IsLibretroReady();
bool LibretroShouldClose();
void LibretroInputPoll();
void LibretroAudioCallback(void *bufferData, unsigned int frames);
size_t LibretroAudioWrite(const int16_t *data, size_t frames);
void LibretroAudioSample(int16_t left, int16_t right);
size_t LibretroAudioSampleBatch(const int16_t *data, size_t frames);
void LibretroInitAudio();
bool LibretroInitAudioVideo();
bool LoadLibretroGame(const char *gameFile);
const char *GetLibretroName();
const char *GetLibretroVersion();
bool DoesLibretroCoreNeedContent();
bool InitLibretro(const char *core);
void DrawLibretroTexture(int posX, int posY, Color tint);
void DrawLibretroV(Vector2 position, Color tint);
void DrawLibretroPro(Rectangle destRec, Color tint);
void DrawLibretroTint(Color tint);
void DrawLibretro();
unsigned GetLibretroWidth();
unsigned GetLibretroHeight();
Texture2D GetLibretroTexture();
bool IsLibretroGameReady();
void ResetLibretro();
void UnloadLibretroGame();
void CloseLibretro();
int LibretroMapRetroLogLevelToTraceLogType(int level);
int LibretroMapRetroKeyToKeyboardKey(int key);
int LibretroMapRetroJoypadButtonToRetroKey(int button);
int LibretroMapRetroJoypadButtonToGamepadButton(int button);
int LibretroMapRetroPixelFormatToPixelFormat(int pixelFormat);

#if defined(__cplusplus)
}
#endif

#endif // RAYLIB_LIBRETRO_RLIBRETRO_H
