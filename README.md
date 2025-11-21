## GO TO
https://www.msys2.org/

## DOWNLOAD THE INSTALLER (based on your architecture)
msys2-x86-20250830.exe \n
msys2-arm64-20250830.exe 

## DOWNLOAD g++
pacman -Syu \n
open up SYSM2 again \n
pacman -Syu \n
pacman -S mingw-w64-ucrt-x86_64-gcc

## ADD TO PATH
C:\msys64\ucrt64\bin

## CHECK THAT WE HAVE g++ (in VSCode or other text editor)
g++ --version

