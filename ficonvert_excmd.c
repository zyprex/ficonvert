#include "ficonvert_excmd.h"
struct _t_excmd {
  char* tag; // cmd tag
  FIBITMAP* (*excmd_fn)(FIBITMAP* dib, char* param);
} excmd_list[] = {
  {"rotate", excmd_rotate},
  {"rescale", excmd_rescale},
  {"bpp", excmd_bpp},
  {"type", excmd_type},
  {"quantize", excmd_quantize},
  {"flip", excmd_flip},
  {"gamma", excmd_gamma},
  {"brightness", excmd_brightness},
  {"contrast", excmd_contrast},
  {"invert", excmd_invert},
  {"crop", excmd_crop},
};
int excmd_list_len = sizeof(excmd_list) / sizeof(struct _t_excmd);

/*
 * -- parser and run --
 * */

FIBITMAP* ex_cmd_run(FIBITMAP* dib, char* str) {
  for (int i = 0; i < excmd_list_len; ++i) {
    int n = strlen(excmd_list[i].tag);
    if (!strncmp(excmd_list[i].tag,str,n)) {
      printf("INFO: find tag '%s'\n", excmd_list[i].tag);
      dib = excmd_list[i].excmd_fn(dib, str+n);
    }
  }
  return dib;
}

FIBITMAP* ex_cmd_parser(FIBITMAP* dib, char* str) {
  printf(">> parse ex_cmd >>\n");
  FIBITMAP* b = NULL;
  while(*str != '\0') {
    char* p = strrchr(str, ',');
    if (p != NULL) {
      printf(">>-- param next -- %s\n", p+1);
      b = ex_cmd_run(dib, p+1);
      *p = '\0';
    }
    else if (*str != '\0') {
      printf(">>-- param last -- %s\n", str);
      b = ex_cmd_run(dib, str);
      *str = '\0';
    }
  }
  return b;
}

/*
 * -- excmd_functions --
 * */
#define RET_DIB2 if(dib2) return dib2; else return dib;
FIBITMAP* excmd_rotate(FIBITMAP* dib, char* param) {
  double angle = atof(param);
  printf("INFO: rotate angle %f\n", angle);
  FIBITMAP* dib2 = FreeImage_Rotate(dib, angle, NULL);
  RET_DIB2;
}

FIBITMAP* excmd_rescale(FIBITMAP* dib, char* param) {
  int w = 0;
  int h = 0;
  FIBITMAP* dib2 = NULL;
  if (strstr(param,":")) {
    FREE_IMAGE_FILTER filter;
    sscanf(param, "%dx%d:%d", &w, &h, &filter);
    if (filter > 5 || filter < 0) filter = 1;
    printf("INFO: rescale to: %dx%d (filter %d)\n", w, h, filter);
    dib2 = FreeImage_Rescale(dib, w, h, filter);
  } else {
    sscanf(param, "%dx%d", &w, &h);
    printf("INFO: rescale to: %dx%d\n", w, h);
    dib2 = FreeImage_Rescale(dib, w, h, FILTER_BICUBIC);
  }
  RET_DIB2;
}

FIBITMAP* excmd_bpp(FIBITMAP* dib, char* param) {
  int bpp = atoi(param);
  printf("INFO: convert to %d bits per pixel\n", bpp);
  FIBITMAP* dib2 = NULL;
  switch (bpp) {
    case 32: dib2 = FreeImage_ConvertTo32Bits(dib); break;
    case 24: dib2 = FreeImage_ConvertTo24Bits(dib); break;
    case 16: dib2 = FreeImage_ConvertTo16Bits555(dib); break;
    case  8: dib2 = FreeImage_ConvertTo8Bits(dib); break;
    case  4: dib2 = FreeImage_ConvertTo4Bits(dib); break;
    case  1: dib2 = FreeImage_ConvertToGreyscale(dib); break;
    default: printf("WARN: bpp should be 1,4,8,16,24,32\n"); break;
  }
  RET_DIB2;
}

FIBITMAP* excmd_type(FIBITMAP* dib, char* param) {
  printf("INFO: convert to '%s' image type\n", param);
  #define PARAM_EQ(S) (!strcmp(S,param))
  FIBITMAP* dib2 = NULL;
  if (PARAM_EQ("BITMAP")) dib2 = FreeImage_ConvertToType(dib, FIT_BITMAP, TRUE);
  if (PARAM_EQ("UINT16")) dib2 = FreeImage_ConvertToType(dib, FIT_UINT16, TRUE);
  if (PARAM_EQ("INT16")) dib2 = FreeImage_ConvertToType(dib, FIT_INT16, TRUE);
  if (PARAM_EQ("UINT32")) dib2 = FreeImage_ConvertToType(dib, FIT_UINT32, TRUE);
  if (PARAM_EQ("INT32")) dib2 = FreeImage_ConvertToType(dib, FIT_INT32, TRUE);
  if (PARAM_EQ("FLOAT")) dib2 = FreeImage_ConvertToType(dib, FIT_FLOAT, TRUE);
  if (PARAM_EQ("DOUBLE")) dib2 = FreeImage_ConvertToType(dib, FIT_DOUBLE, TRUE);
  if (PARAM_EQ("COMPLEX")) dib2 = FreeImage_ConvertToType(dib, FIT_COMPLEX, TRUE);
  if (PARAM_EQ("RGB16")) dib2 = FreeImage_ConvertToType(dib, FIT_RGB16, TRUE);
  if (PARAM_EQ("RGBA16")) dib2 = FreeImage_ConvertToType(dib, FIT_RGBA16, TRUE);
  if (PARAM_EQ("RGBF")) dib2 = FreeImage_ConvertToType(dib, FIT_RGBF, TRUE);
  if (PARAM_EQ("RGBAF")) dib2 = FreeImage_ConvertToType(dib, FIT_RGBAF, TRUE);
  RET_DIB2

}

FIBITMAP* excmd_quantize(FIBITMAP* dib, char* param) {
  int fiq = atoi(param);
  if (fiq > 2 || fiq < 0) fiq = 0;
  // 0=FIQ_WUQUANT 1=FIQ_NNQUANT 2=FIQ_LFPQUANT
  FIBITMAP* dib2 = FreeImage_ColorQuantize(dib, fiq);
  printf("INFO: use quantize\n");
  RET_DIB2;
}

FIBITMAP* excmd_flip(FIBITMAP* dib, char* param) {
  if (param[0] == 'V') {
    FreeImage_FlipVertical(dib);
    printf("INFO: flip vertical\n");
  }
  if (param[0] == 'H') {
    FreeImage_FlipHorizontal(dib);
    printf("INFO: flip horizontal\n");
  }
  return dib;
}


FIBITMAP* excmd_gamma(FIBITMAP* dib, char* param) {
  double gamma = atof(param);
  if (FreeImage_AdjustGamma(dib, gamma)) {
    printf("INFO: gamma correction %f\n", gamma);
  } else {
    printf("WARN: gamma correction %f failed\n", gamma);
  }
  return dib;
}

FIBITMAP* excmd_brightness(FIBITMAP* dib, char* param) {
  double brightness = atof(param);
  if (FreeImage_AdjustBrightness(dib, brightness)) {
    printf("INFO: set brightness %f %%\n", brightness);
  } else {
    printf("WARN: set brightness %f %% failed\n", brightness);
  }
  return dib;
}

FIBITMAP* excmd_contrast(FIBITMAP* dib, char* param) {
  double contrast = atof(param);
  if (FreeImage_AdjustContrast(dib, contrast)) {
    printf("INFO: set contrast %f %%\n", contrast);
  } else {
    printf("WARN: set contrast %f %% failed\n", contrast);
  }
  return dib;
}

FIBITMAP* excmd_invert(FIBITMAP* dib, char* param) {
  FreeImage_Invert(dib);
  printf("INFO: invert image %s\n", param);
  return dib;
}

FIBITMAP* excmd_crop(FIBITMAP* dib, char* param) {
  int l, t, r, b;
  double sl, st, sr, sb;
  int w = FreeImage_GetWidth(dib);
  int h = FreeImage_GetHeight(dib);

  sscanf(param, "%lf:%lf:%lf:%lf", &sl, &st, &sr, &sb);


  if (sl < 1.0) sl *= w;
  if (sr < 1.0) sr *= w;
  if (st < 1.0) st *= h;
  if (sb < 1.0) sb *= h;

  printf("INFO: crop delt %.2lf %.2lf %.2lf %.2lf\n", sl, st, sr, sb);

  l = (int)sl;
  t = (int)st;
  r = (int)(w - sr);
  b = (int)(h - sb);

  printf("INFO: crop rect %d %d %d %d\n", l, t, r, b);

  FIBITMAP* dib2 = NULL;
  dib2 = FreeImage_Copy(dib, l, t, r, b);
  RET_DIB2;
}

