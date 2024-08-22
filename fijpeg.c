#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "FreeImage.h"
void print_help(char* prog) {
  printf(
      "Description: lossless jpeg transformation tool.\n"
      "Usage: %s <input-jpg> <output-jpg> [left] [top] [right] [bottom] [jpeg_operation]\n"
      "\n"
      "jpeg_operation:\n"
      "\tfilpH -- Horizontal flip\n"
      "\tfilpV -- Vertical flip\n"
      "\ttranspose -- Transpose across upper-left to lower-right axis\n"
      "\ttransverse -- Transpose across upper-right to lower-left axis\n"
      "\trotate90 -- 90-degree clockwise rotation\n"
      "\trotate180 -- 180-degree clockwise rotation\n"
      "\trotate270 -- 270-degree clockwise rotation\n"
      "\n"
      "Example: %s input.jpg output.jpg 0.2 100 0.2 100 rotate270,\n"
      "crop left 20%%, top 100px right 20%%, bottom 100px, and rotate 270 degree."
      , prog, prog);
}
int main(int argc, char *argv[]) {
  if (argc <= 3) {
    print_help(argv[0]);
    return 0;
  }
  FREE_IMAGE_JPEG_OPERATION jpeg_operation = FIJPEG_OP_NONE;

  FreeImage_Initialise(FALSE);
  FIBITMAP* dib = FreeImage_Load(FIF_JPEG, argv[1], 0);
  int w = FreeImage_GetWidth(dib);
  int h = FreeImage_GetHeight(dib);
  FreeImage_Unload(dib);

  int l = 0, r = 0, t = 0, b = 0;
  double dl = 0.0, dr = 0.0, dt = 0.0, db = 0.0;
  if (argc >= 4) { dl = atof(argv[3]); l = (int)((dl < 1.0) ? dl * w : dl); }
  if (argc >= 5) { dt = atof(argv[4]); t = (int)((dt < 1.0) ? dt * h : dt); }
  if (argc >= 6) { dr = atof(argv[5]); r = (int)((dr < 1.0) ? dr * w : dr); }
  if (argc >= 7) { db = atof(argv[6]); b = (int)((db < 1.0) ? db * h : db); }
#define OP_IN(x) if (!strcmp(argv[7], x)) jpeg_operation = 
  if (argc >= 8) {
OP_IN("flipH")FIJPEG_OP_FLIP_H;
OP_IN("flipV")FIJPEG_OP_FLIP_V;
OP_IN("transpose")FIJPEG_OP_TRANSPOSE;
OP_IN("transverse")FIJPEG_OP_TRANSVERSE;
OP_IN("rotate90")FIJPEG_OP_ROTATE_90;
OP_IN("rotate180")FIJPEG_OP_ROTATE_180;
OP_IN("rotate270")FIJPEG_OP_ROTATE_270;
  }
#undef OP_IN
  FreeImage_JPEGTransformCombined(argv[1], argv[2], jpeg_operation, &l, &t, &r, &b, 1);
  FreeImage_DeInitialise();
  return 0;
}
