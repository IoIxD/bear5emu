#include "../raylib-libretro/raylib-libretro.h"
#include "configs.hpp"
#include <stdio.h>

namespace configs {
Position SMB1Config::MarioPos() {
  void *memory = GetSystemMemory();
  unsigned char px = ((unsigned char *)memory)[0x0086];
  unsigned char py = (float)(((unsigned char *)memory)[0x00CE]);
  unsigned char lpos = ((unsigned char *)memory)[0x006D];
  unsigned char pv = (float)(((unsigned char *)memory)[0x00B5]);

  float level_progress = (255.0 * (float)(lpos));
  float x = ((float)(px) + level_progress);
  float y = ((float)py + (float)pv);

  return Position(x, y);
}
Position SMB1Config::MarioScreenPos() {
  void *memory = GetSystemMemory();
  unsigned char pox = ((unsigned char *)memory)[0x03AD];
  unsigned char poy = ((unsigned char *)memory)[0x03B8];

  return Position(pox, poy);
}
bool SMB1Config::LevelEnding() {
  void *memory = GetSystemMemory();
  unsigned char state = ((unsigned char *)memory)[0x000E];
  bool cutscene = ((unsigned char *)memory)[0x0723] != 0;
  return cutscene || (state == 0x04 || state == 0x05 || state == 0x07);
}
bool SMB1Config::Bear5CanAttack() {
  void *memory = GetSystemMemory();
  unsigned char paused = ((unsigned char *)memory)[0x0776];
  unsigned char prelevel = ((unsigned char *)memory)[0x075E];

  return !paused && prelevel != 1;
};
int SMB1Config::Level() { return ((unsigned char *)GetSystemMemory())[0x0760]; }
bool SMB1Config::Bear5AttackOverride() {
  /*void *memory = GetSystemMemory();
  // bool hasLuigi = ((unsigned char *)memory)[0x077A] == 1;
  unsigned char warp_zone_offset_1 = ((unsigned char *)memory)[0x87F2];
  unsigned char warp_zone_offset_2 = ((unsigned char *)memory)[0x87F3];
  unsigned char warp_zone_offset_3 = ((unsigned char *)memory)[0x87F4];
  bool enteredWarpZone =
      (warp_zone_offset_1 != 0 && warp_zone_offset_1 != 255 &&
       warp_zone_offset_2 != 0 && warp_zone_offset_2 != 255 &&
       warp_zone_offset_3 != 0 && warp_zone_offset_3 != 255);
  return enteredWarpZone;*/
  return false;
};

float SMB1Config::Speed() { return 2.0; }

bool SMB1Config::Music() {
  void *memory = GetSystemMemory();
  // auto aboveGround = ((unsigned char *)memory)[0x0773] == 1;
  return ((unsigned char *)memory)[0x0770] == 1 &&
         ((unsigned char *)memory)[0x0772] == 3;
}
} // namespace configs