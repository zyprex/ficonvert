#pragma once
#include "FreeImage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

FIBITMAP* ex_cmd_parser(FIBITMAP* dib, char* str);
FIBITMAP* ex_cmd_run(FIBITMAP* dib, char* str);

FIBITMAP* excmd_rotate(FIBITMAP* dib, char* param);
FIBITMAP* excmd_rescale(FIBITMAP* dib, char* param);
FIBITMAP* excmd_bpp(FIBITMAP* dib, char* param);
FIBITMAP* excmd_type(FIBITMAP* dib, char* param);
FIBITMAP* excmd_quantize(FIBITMAP* dib, char* param);
FIBITMAP* excmd_flip(FIBITMAP* dib, char* param);

FIBITMAP* excmd_gamma(FIBITMAP* dib, char* param);
FIBITMAP* excmd_brightness(FIBITMAP* dib, char* param);
FIBITMAP* excmd_contrast(FIBITMAP* dib, char* param);
FIBITMAP* excmd_invert(FIBITMAP* dib, char* param);

FIBITMAP* excmd_crop(FIBITMAP* dib, char* param);
