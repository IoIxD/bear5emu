#include "images/images.h"
#include "raylib.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef struct Bear5Struct {
  float x;
  float y;
} Bear5Struct;

extern Bear5Struct Bear5;

Texture InitBear5();

#define CREATE_JUMPSCARE_CREATOR(num, __width, __height, __format, __data)     \
  Texture InitJumpscare##num() {                                               \
    Image img;                                                                 \
    img.width = __width;                                                       \
    img.height = __height;                                                     \
    img.format = __format;                                                     \
    img.data = __data;                                                         \
    img.mipmaps = 1;                                                           \
    return LoadTextureFromImage(img);                                          \
  }

Texture InitJumpscare1();
Texture InitJumpscare2();
Texture InitJumpscare3();
Texture InitJumpscare4();
#ifdef __cplusplus
}
#endif