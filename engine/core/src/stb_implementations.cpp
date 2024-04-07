#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#pragma GCC diagnostic ignored "-Wmissing-declarations"
#define STB_PERLIN_IMPLEMENTATION
#include <stb_perlin.h>

// Microprofile seems to implement this already
#if MICROPROFILE_ENABLED == 0
#define STB_SPRINTF_IMPLEMENTATION
#include <stb_sprintf.h>
#endif
