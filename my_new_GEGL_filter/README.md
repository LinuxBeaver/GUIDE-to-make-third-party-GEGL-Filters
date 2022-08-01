 Making third party GEGL filters should not be practied until one has a good understanding of GEGL syntax. You can learn GEGL syntax at GEGL.org and on the Gimp Chat GEGL threads. All third party GEGL filters you make can follow this C file as a template. Just download GEGL's source code https://github.com/GNOME/gegl and Navigate to the directory Common to find all the filters and parameters of filters that you will need to define.  

Credit goes to https://github.com/4Evergreen4/pixelsort for making most of this README markdown and
making the first third party GEGL plugin that I am aware of.

## Compiling and Installing
The .so/dll file/files of the third party GEGL Filters binaries goes in /gegl-0.4/plug-ins/

### Linux

To compile and install you will need the GEGL header files (`libgegl-dev` on
Debian based distributions or `gegl` on Arch Linux) and meson (`meson` on
most distributions).

```bash
meson setup --buildtype=release build
ninja -C build

```

If you have an older version of gegl you may need to copy to `~/gegl-0.3/plug-ins`
instead (on Ubuntu 18.04 for example).



### Windows

The easiest way to compile this project on Windows is by using msys2.  Download
and install it from here: https://www.msys2.org/

Open a msys2 terminal with `C:\msys64\mingw64.exe`.  Run the following to
install required build dependencies:

```bash
pacman --noconfirm -S base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-meson mingw-w64-x86_64-gegl
```

Then build the same way you would on Linux:

```bash
meson setup --buildtype=release build
ninja -C build
```



