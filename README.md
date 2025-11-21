
# GO TO
https://www.msys2.org/

# DOWNLOAD THE INSTALLER
msys2-x86-20250830.exe (Windows)
msys2-arm64-20250830.exe (MacOS)

# DOWNLOAD GCC
pacman -Syu
open up SYSM2 again
pacman -Syu
pacman -S mingw-w64-ucrt-x86_64-gcc (Windows)

# ADD TO PATH
C:\msys64\ucrt64\bin

# CHECK THAT WE HAVE g++
g++ --version

