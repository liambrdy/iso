@echo off

curl -fsSL -o SDL2-devel-2.26.3-VC.zip https://www.libsdl.org/release/SDL2-devel-2.26.3-VC.zip
tar -xf SDL2-devel-2.26.3-VC.zip
if not exist dependencies\ mkdir dependencies\
move SDL2-2.26.3 dependencies\SDL2
del SDL2-devel-2.26.3-VC.zip
if not exist dependencies\SDL2\temp\ mkdir dependencies\SDL2\temp\
move dependencies\SDL2\include dependencies\SDL2\temp\SDL2
move dependencies\SDL2\temp dependencies\SDL2\include
if not exist build\ mkdir build\
copy dependencies\SDL2\lib\x64\SDL2.dll build\SDL2.dll

curl -fsSL -o glew-2.1.0-win32.zip https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0-win32.zip/download
tar -xf glew-2.1.0-win32.zip
if not exist dependencies\GLEW\lib\ mkdir dependencies\GLEW\lib\
move glew-2.1.0\lib\Release\x64\glew32s.lib dependencies\GLEW\lib\glew32s.lib
if not exist dependencies\GLEW\include\GL\ mkdir dependencies\GLEW\include\GL\
move glew-2.1.0\include\GL\glew.h dependencies\GLEW\include\GL\glew.h
del glew-2.1.0-win32.zip
rmdir /s /q glew-2.1.0