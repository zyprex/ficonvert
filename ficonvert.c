#include "ficonvert.h"
#include "ficonvert_excmd.h"

char* old_name = NULL;
FREE_IMAGE_FORMAT old_fmt = FIF_UNKNOWN;
char* new_name = NULL;
FREE_IMAGE_FORMAT new_fmt = FIF_UNKNOWN;
int new_flag = 0;

int flag_detect(char* str) {
  for (int i = 0; i < stf_list_len; ++i) {
     if (!strcmp(stf_list[i].s, str)) {
       printf("set output flag '%s'\n", str);
       return stf_list[i].flag;
     }
  }
  int rate = atoi(str);
  if (rate >= 0 && rate <= 512) {
       printf("set output flag rate %d\n", rate);
    return rate;
  }
  return 0;
}

char* image_format_to_string(FREE_IMAGE_FORMAT fif) {
  for(int i = 0; i < stfif_list_len; i++) {
    if (fif == stfif_list[i].fif) {
      return stfif_list[i].s;
    }
  }
  return NULL;
}

FREE_IMAGE_FORMAT image_format_detect(char* str) {
  FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
  fif = FreeImage_GetFileType(str, 0);
  if (fif == FIF_UNKNOWN) {
    fif = FreeImage_GetFIFFromFilename(str);
  }
  return fif;
}

char* image_format_color_type(FIBITMAP* dib) {
  FREE_IMAGE_COLOR_TYPE ct = FreeImage_GetColorType(dib);
  for (int i = 0; i < cts_list_len; ++i) {
     if (ct == cts_list[i].ct)
       return cts_list[i].s;
  }
  return NULL;
}

void print_error_save_failed() {
    printf("WARN: save failed: %s -> %s\n",
        image_format_to_string(old_fmt),
        image_format_to_string(new_fmt));
}

void print_info_save_success() {
    printf("INFO: save success: %s -> %s\n",
        image_format_to_string(old_fmt),
        image_format_to_string(new_fmt));
}

void print_info_only_input(FIBITMAP* dib) {
  printf("================================\n");
  printf("FreeImage Version: %s\n", FreeImage_GetVersion());
  unsigned bpp = FreeImage_GetBPP(dib); // 1 4 8 16 24 32
  unsigned w = FreeImage_GetWidth(dib);
  unsigned h = FreeImage_GetHeight(dib);
  printf("Input Image Info: %dx%d %s(%d)\n", w, h, image_format_color_type(dib), bpp);
  printf("================================\n");
}


int main(int argc, char *argv[]) {

  if (argc < 2) {
    printf(
        "Usage:\n"
        "%s <input> <output> [output_flag] [extra_cmd]\n\n"
        "\t[output_flag] see `doc/encoder-opts-0.png`\n\n"
        "\t[extra_cmd] multiple cmd seperated by ','\n"
        "\t|CMD|params|example|\n"
        "\t|rotate|DOUBLE:0.0~360.0|rotate90|\n"
        "\t|rescale[1]|INT:WxH:0~5|rescale8x8,rescale32x32:5|\n"
        "\t|bpp|INT:N|bpp24|\n"
        "\t|type|STR:[3]|typeFLOAT|\n"
        "\t|quantize[2]|N:0,1,2|quantize,quantize2|\n"
        "\t|flip|CHAR:V,H|flipV|\n"
        "\t|gamma|DOUBLE:0.0~|gamma1.1|\n"
        "\t|brightness|DOUBLE:-100~100|brightness-50|\n"
        "\t|contrast|DOUBLE:-100~100|contrast80|\n"
        "\t|invert||invert|\n"
        "\n"
        "\t[1]: 0=FILTER_BOX,1=FILTER_BICUBIC,2=FILTER_BILINEAR,\n3=FILTER_BSPLINE,4=FILTER_CATMULLROM,5=FILTER_LANCZOS3 \n"
        "\t[2]: 0=FIQ_WUQUANT,1=FIQ_NNQUANT,2=FIQ_LFPQUANT\n"
        "\t[3]: BITMAP,UINT16,INT16,UINT32,INT32,FLOAT,DOUBLE,COMPLEX,RGB16,RGBA16,RGBF,RGBAF\n"
        "\n"
        "\tTips:\n"
        "\t1. GIF need downgrade bpp to 8, use quantize to keep color\n"
        "\t2. ICO should rescale to same width and height.\n"
        "\t3. JPEG's bpp = 24 (no alpha channel).\n"
        "\t4. Some extra cmd cannot be combined, e.g. bpp, rescale.\n"
        "\t5. First extra cmd will execute at last, so the cmd order is important.\n"

        , argv[0]);
    return 0;
  }

  FreeImage_Initialise(FALSE);

  FIBITMAP* dib = NULL;

  /*
   * load input image
   * */

  old_name = argv[1];
  old_fmt = image_format_detect(old_name);

  if (FreeImage_FIFSupportsReading(old_fmt)) {
    dib = FreeImage_Load(old_fmt, old_name, 0);
  } else {
    printf("ERROR: can't read this image format %s\n", image_format_to_string(old_fmt));
    goto prog_end;
  }

  /*
   * define output image
   * */
  if (argc < 3) {
    print_info_only_input(dib);
    goto prog_end_pre;
  }

  new_name = argv[2];
  new_fmt = image_format_detect(new_name);

  if (!FreeImage_FIFSupportsWriting(new_fmt)) {
    printf("ERROR: can't wirte this image format %s\n", image_format_to_string(new_fmt));
    goto prog_end_pre;
  }

  printf("================================\n");
  printf("%s -> %s\n", old_name, new_name);

  if (argc > 3) {
    new_flag = flag_detect(argv[3]);
  }

  printf("================================\n");

  
  if (argc > 4) {
    dib = ex_cmd_parser(dib, argv[4]);
  }

  if(!FreeImage_Save(new_fmt, dib, new_name, new_flag)) {
    print_error_save_failed();
  } else {
    print_info_save_success();
  }

prog_end_pre:
  FreeImage_Unload(dib);

prog_end:
  FreeImage_DeInitialise();

  return 0;
}
