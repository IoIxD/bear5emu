#include "bear5.h"

#ifdef __cplusplus
extern "C" {
#endif
Bear5Struct Bear5 = {0};

Texture InitBear5() {
  Bear5.x = 0.0;
  Bear5.y = 0.0;
  Image img;
  img.width = BEAR5_WIDTH;
  img.height = BEAR5_HEIGHT;
  img.format = BEAR5_FORMAT;
  img.data = BEAR5_DATA;
  img.mipmaps = 1;
  return LoadTextureFromImage(img);
}

Texture InitJumpscare1() {
  Image img;
  img.width = JUMPSCARE1_WIDTH;
  img.height = JUMPSCARE1_HEIGHT;
  img.format = JUMPSCARE1_FORMAT;
  img.data = JUMPSCARE1_DATA;
  img.mipmaps = 1;
  return LoadTextureFromImage(img);
};
Texture InitJumpscare2() {
  Image img;
  img.width = JUMPSCARE2_WIDTH;
  img.height = JUMPSCARE2_HEIGHT;
  img.format = JUMPSCARE2_FORMAT;
  img.data = JUMPSCARE2_DATA;
  img.mipmaps = 1;
  return LoadTextureFromImage(img);
};
Texture InitJumpscare3() {
  Image img;
  img.width = JUMPSCARE3_WIDTH;
  img.height = JUMPSCARE3_HEIGHT;
  img.format = JUMPSCARE3_FORMAT;
  img.data = JUMPSCARE3_DATA;
  img.mipmaps = 1;
  return LoadTextureFromImage(img);
};
Texture InitJumpscare4() {
  Image img;
  img.width = JUMPSCARE4_WIDTH;
  img.height = JUMPSCARE4_HEIGHT;
  img.format = JUMPSCARE4_FORMAT;
  img.data = JUMPSCARE4_DATA;
  img.mipmaps = 1;
  return LoadTextureFromImage(img);
};

#ifdef __cplusplus
}
#endif