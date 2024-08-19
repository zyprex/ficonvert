# About

`ficonvert` is a free image convert command line interface tool which use the FreeImage library.

The program is proposed to integrated in scripts seamlessly.


# Get help

Just type `ficonvert` in terminal.

# Usage, Examples and Some Explain

1. Convert image, webp to png (the simplest way).

```
ficonvert sample.webp sample.png
```

2. Convert image and apply an output flag to change the image quality.
Be aware of input image's BPP (bits per pixel), some image format type should be converted
correctly, we use the extra cmd  `bpp24` because PNG's bpp could be 32.

```
ficonvert sample.png sample.jpg JPEG_QUALITYGOOD bpp24
```

3. Convert image and apply some image affects.

```
ficonvert sample.jpg sample.tiff 0 rotate14.4,flipV,gamma0.1,invert
```

4. Convert image jpg to ico.

```
ficonvert sample.jpg sample.ico 0 rescale32x32
```

5. Convert image jpg to gif.

```
ficonvert sample.jpg sample.gif 0 quantize
```

Or try to convert to 8 bpp directly.

```
ficonvert sample.jpg sample.gif 0 bpp8
```

6. convert all images in one directory.

For batch script:

```bat
@ECHO OFF
SET fic=ficonvert.exe
:: Set delims empty, split new line only
FOR /F "delims="  %%F IN ('DIR/B/S/A:-D *.png') DO (
    :: test file
    :: echo %%F
    :: after test
    :: %fic% %%F %%~dpnF.jpg JPEG_QUALITYGOOD
)
```

For bash script:

```sh
#!/bin/bash
fic="ficonvert"
for i in $(find . -type f -name *.png); do
    # test file
    # echo $i
    # after test
    # ${fic} $i ${i%.*}.jpg JPEG_QUALITYGOOD
done
```

7. crop one image to half.

```
ficonvert test.jpg left.jpg 0 crop0:0:0.5:0
ficonvert test.jpg right.jpg 0 crop0.5:0:0:0
```

# Notice

For more details, please read the code and refer to FreeImage's document.

Keep in mind, the converter will overwrite any exist file, test before run !

