# USAGE

## CHECK THAT WE HAVE g++ (in VSCode or other text editor)
g++ --version <br>
INSTRUCTIONS TO DOWNLOAD BELOW

## CREATE THE RUNNABLE PROGRAM FROM YOUR SOURCE CODE
g++ compiler.cpp -o compiler

## START THE TOKENIZER
./compiler test.txt

# DOWNLOADING g++

## GO TO
https://www.msys2.org/

## DOWNLOAD THE INSTALLER (based on your architecture)
msys2-x86-20250830.exe <br>
msys2-arm64-20250830.exe 

## DOWNLOAD g++
pacman -Syu <br>
open up SYSM2 again <br>
pacman -Syu <br>
pacman -S mingw-w64-ucrt-x86_64-gcc

## ADD TO PATH
C:\msys64\ucrt64\bin

## CHECK THAT WE HAVE g++ (in VSCode or other text editor)
g++ --version