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
  void *memory = GetSystemMemory();
  bool hasLuigi = ((unsigned char *)memory)[0x077A] == 1;
  unsigned char warp_zone_offset = ((unsigned char *)memory)[0x06D6];
  bool enteredWarpZone = (warp_zone_offset != 0 && warp_zone_offset != 255);
  return hasLuigi || enteredWarpZone;
};

float SMB1Config::Speed() { return 2.0; }
} // namespace configs