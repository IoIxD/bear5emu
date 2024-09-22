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

int cur_level = 0;

int main(int argc, char *argv[]) {
  // Ensure proper amount of arguments.
  // SetTraceLogExit(LOG_FATAL);

  // Create the window and audio.
  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(512, 448, "raylib-libretro");
  SetWindowMinSize(400, 300);
  InitAudioDevice();

  // Load the shaders and the menu.
  InitMenu();
  InitLibretro("./libraries/fceumm.so");

  SetTargetFPS(60);

  Texture Bear5Texture = InitBear5();

  Texture j1 = InitJumpscare1();
  Texture j2 = InitJumpscare2();
  Texture j3 = InitJumpscare3();
  Texture j4 = InitJumpscare4();

  float dx, dx_;

  configs::GameConfig *config = new configs::SMB1Config();
  while (!WindowShouldClose()) {
    if (!IsLibretroReady()) {
      continue;
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
    printf("Level %d, %0.2f %0.2f\n", config->Level(), x, y);
    // Release Bear 5 if the player reaches the appropriate part of the level.
    if (config->Level() == 0) {
      if (x >= 1650.0 && x <= 3450.0 && bearState == BEAR_5_IS_GONE) {
        bearState = BEAR_5_WILL_COME;
      }
    }

    // Reset Bear 5 if the level changes
    if (cur_level != config->Level()) {
      Bear5.x = -1000.0;
      Bear5.y = -1000.0;
      cur_level = config->Level();
    }

    if (!IsMenuActive() && bearState != BEAR_5_WILL_COME) {
      // Run a frame of the core.
      UpdateLibretro();
    }

    // Check if the core asks to be shutdown.
    if (LibretroShouldClose()) {
      UnloadLibretroGame();
      CloseLibretro();
    }

    // Render the libretro core.
    BeginDrawing();
    {
      ClearBackground(BLACK);

      UpdateMenu();
      DrawLibretro();
      if (bearState == BEAR_5_WILL_COME) {
        DrawText("BEAR 5 IS COMING",
                 GetScreenWidth() / 2 -
                     (MeasureText("BEAR 5 IS COMING", 30) / 2),
                 GetScreenHeight() / 2, 30, RED);
        bear5Timer += 1;
        Bear5.x = x - 384.0;
        Bear5.y = y - 384.0;
        if (bear5Timer >= 240) {
          bearState = BEAR_5_IS_COMING;
        }
      }

      dx = (Bear5.x - x);

      int offset = (float)(GetScreenWidth()) *
                   (((float)sx / (float)GetScreenWidth()) * 2.0);

      float dy = (float)(Bear5.y * 2) - 16;
      if (bearState == BEAR_5_IS_COMING && config->Bear5CanAttack()) {
        DrawTexturePro(Bear5Texture,
                       (Rectangle){0, 0, BEAR5_WIDTH, BEAR5_HEIGHT},
                       (Rectangle){dx + offset - 16, dy, 64, 64}, Vector2Zero(),
                       0.0, WHITE);

        int b_dx = (ceil(dx + offset));
        int p_dx = (sx * 2);
        int b_dy = ceil(dy / 2.0);
        int p_dy = (sy - 6);
        if ((b_dx >= p_dx - 3 && b_dx <= p_dx + 3) &&
            (b_dy >= p_dy - 3 && b_dy <= p_dy + 3)) {
          if (config->LevelEnding()) {
            Bear5.x = -100.0;
            Bear5.y = -100.0;
          } else {
            bearState = BEAR_5_KILLED_YOU;
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
        exit(0);
      };
    }

    EndDrawing();

    // Fullscreen
    if (IsKeyReleased(KEY_F11)) {
      ToggleFullscreen();
    }
  }

  // Unload the game and close the core.
  UnloadLibretroGame();
  CloseLibretro();

  CloseAudioDevice();
  CloseWindow();

  return 0;
}
