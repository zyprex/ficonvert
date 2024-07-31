@echo off
set test_file_name=sheep
set fic=..\ficonvert.exe sheep.png sheep.

:: test cmd
:: %fic%transform.png PNG_Z_BEST_SPEED rotate45.5,flipV
:: %fic%repaint.png PNG_Z_BEST_SPEED contrast100
:: %fic%repaint2.png PNG_Z_BEST_SPEED contrast100,gamma0.5

:: common image file
:: set ext_list=bmp webp tiff psd
:: (for %%a in (%ext_list%) do ( 
   :: %fic%%%a 
:: ))
:: %fic%jpg 0 bpp24
:: %fic%gif 0 quantize
:: %fic%ico 0 rescale64x64
:: ..\ficonvert.exe sheep.ico sheep.xpm PNM_SAVE_ASCII

:: other image file
:: %fic%exr 0 typeFLOAT
:: %fic%hdr 0 typeRGBF
:: %fic%j2k 0 typeBITMAP
:: %fic%jng
:: %fic%jxr
:: %fic%pbm 0 bpp1
:: %fic%tga

