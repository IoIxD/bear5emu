#include "bear5.h"
#include "configs/configs.hpp"
#include "raylib-libretro/menu.h"
#include "raylib-libretro/raylib-libretro.h"
#include "raylib.h"
#include <raymath.h>
#include <stdio.h>

typedef enum Bear5State {
  BEAR_5_IS_GONE,
  BEAR_5_WILL_COME,
  BEAR_5_IS_COMING,
  BEAR_5_KILLED_YOU
} Bear5State;

Bear5State bearState = BEAR_5_IS_GONE;
int bear5Timer = 0;
int killTimer = 0;
bool overworldMusicPlayed = false;

int cur_level = -1;

void BackToMenu() {
  ResetLibretro();
  killTimer = 0;
  bear5Timer = 0;
  cur_level = -1;
  bearState = BEAR_5_IS_COMING;
  Bear5.x = 0.0;
  Bear5.y = 0.0;
}

int main(int argc, char *argv[]) {
  // Ensure proper amount of arguments.
  // SetTraceLogExit(LOG_FATAL);

  // Create the window and audio.
  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(512, 448, "raylib-libretro");
  SetWindowMinSize(400, 300);
  InitAudioDevice();
  auto mainMusic = LoadMusicStream("music/1-1.mp3");
  auto simpsons = LoadMusicStream("music/simpsons.wav");

  // Load the shaders and the menu.
  InitMenu();
#ifdef _WIN32
  InitLibretro("./libraries/fceumm.dll");
#else
  InitLibretro("./libraries/fceumm.so");
#endif

  SetTargetFPS(60);

  Texture Bear5Texture = InitBear5();

  Texture j1 = InitJumpscare1();
  Texture j2 = InitJumpscare2();
  Texture j3 = InitJumpscare3();
  Texture j4 = InitJumpscare4();

  configs::GameConfig *config = new configs::SMB1Config();
  while (!WindowShouldClose()) {
    if (!IsLibretroReady()) {
      continue;
    }

    if (bearState == BEAR_5_IS_GONE) {
      if (config->Music()) {
        if (!overworldMusicPlayed) {
          PlayMusicStream(mainMusic);
          overworldMusicPlayed = true;
        } else {
          UpdateMusicStream(mainMusic);
        }
      }
    } else {
      UpdateMusicStream(simpsons);
    }

    auto pos = config->MarioPos();
    auto spos = config->MarioScreenPos();

    auto x = pos.x;
    auto y = pos.y;
    auto sx = spos.x;
    auto sy = spos.y;

    if (config->Bear5AttackOverride()) {
      bearState = BEAR_5_KILLED_YOU;
    }
    // Release Bear 5 if the player reaches the appropriate part of the level.
    if (config->Level() == 0) {
      if (x >= 1650.0 && x <= 3450.0 && bearState == BEAR_5_IS_GONE) {
        bearState = BEAR_5_WILL_COME;
      }
    } else if (config->Level() <= 64 && bearState == BEAR_5_IS_GONE) {
      bearState = BEAR_5_WILL_COME;
    }

    // Reset Bear 5 if the level changes
    if (cur_level != config->Level()) {
      Bear5.x = -250.0;
      Bear5.y = -250.0;
      cur_level = config->Level();
    }

    if (!IsMenuActive() && bearState != BEAR_5_WILL_COME) {
      // Run a frame of the core.
      UpdateLibretro();
    }

    // Render the libretro core.
    BeginDrawing();
    {
      ClearBackground(BLACK);

      UpdateMenu();

      DrawLibretro();

      if (bearState == BEAR_5_WILL_COME) {
        StopMusicStream(mainMusic);
        DrawText("BEAR 5 IS COMING",
                 GetScreenWidth() / 2 -
                     (MeasureText("BEAR 5 IS COMING", 30) / 2),
                 GetScreenHeight() / 2, 30, RED);
        bear5Timer += 1;
        Bear5.x = x - 250.0;
        Bear5.y = y - 250.0;
        if (bear5Timer >= 240) {
          bearState = BEAR_5_IS_COMING;
        }
      }

      float screen_x = (Bear5.x - x);

      int offset = (float)(GetScreenWidth()) *
                   (((float)sx / (float)GetScreenWidth()) * 2.0);

      float screen_y = (float)(Bear5.y * 2) - 16;
      if (bearState == BEAR_5_IS_COMING) {
        PlayMusicStream(simpsons);
        if (config->Bear5CanAttack()) {
          DrawTexturePro(Bear5Texture,
                         (Rectangle){0, 0, BEAR5_WIDTH, BEAR5_HEIGHT},
                         (Rectangle){screen_x + offset - 16, screen_y, 64, 64},
                         Vector2Zero(), 0.0, WHITE);

          int bear_screen_x = (ceil(screen_x + offset));
          int player_screen_x = (sx * 2);
          int bear_screen_y = ceil(screen_y / 2.0);
          int player_screen_y = (sy - 6);

          if (bear_screen_x >= 5 && bear_screen_y >= 5) {
            if ((bear_screen_x >= player_screen_x - 3 &&
                 bear_screen_x <= player_screen_x + 3) &&
                (bear_screen_y >= player_screen_y - 3 &&
                 bear_screen_y <= player_screen_y + 3)) {
              if (config->LevelEnding()) {
                Bear5.x = -250.0;
                Bear5.y = -250.0;
              } else {
                bearState = BEAR_5_KILLED_YOU;
              }
            }
          }
        }
      }
    }

    if (config->Bear5CanAttack()) {
      // Bring Bear 5 closer.
      if (Bear5.x >= x) {
        Bear5.x -= config->Speed();
      };
      if (Bear5.x <= x) {
        Bear5.x += config->Speed();
      }
      if (Bear5.y >= y) {
        Bear5.y -= config->Speed();
      };
      if (Bear5.y <= y) {
        Bear5.y += config->Speed();
      }
    }

    if (bearState == BEAR_5_KILLED_YOU) {
      killTimer += 1;
      float scrwidth = (float)(GetScreenWidth());
      float scrheight = (float)(GetScreenHeight());
      if (killTimer <= 60) {
        DrawTexturePro(j1, (Rectangle){0, 0, float(j1.width), float(j1.height)},
                       (Rectangle){0, 0, scrwidth, scrheight}, Vector2Zero(),
                       0.0, WHITE);
      };
      if (killTimer >= 60 && killTimer <= 240) {
        DrawTexturePro(j2, (Rectangle){0, 0, float(j2.width), float(j2.height)},
                       (Rectangle){0, 0, scrwidth, scrheight}, Vector2Zero(),
                       0.0, WHITE);
      };

      if (killTimer >= 240 && killTimer <= 300) {
        DrawTexturePro(j3, (Rectangle){0, 0, float(j3.width), float(j3.height)},
                       (Rectangle){0, 0, scrwidth, scrheight}, Vector2Zero(),
                       0.0, WHITE);
      };

      if (killTimer >= 300) {
        DrawTexturePro(j4, (Rectangle){0, 0, float(j4.width), float(j4.height)},
                       (Rectangle){0, 0, scrwidth, scrheight}, Vector2Zero(),
                       0.0, WHITE);
      };
      if (killTimer >= 400) {
        BackToMenu();
      };
    }

    EndDrawing();

    // Fullscreen
    if (IsKeyReleased(KEY_F11)) {
      ToggleFullscreen();
    }

    // Check if the core asks to be shutdown.
    if (LibretroShouldClose()) {
      UnloadLibretroGame();
      CloseLibretro();
    }

    if (IsKeyReleased(KEY_Q)) {
      BackToMenu();
    }
  }

  CloseLibretro();

  CloseAudioDevice();
  CloseWindow();

  return 0;
}
