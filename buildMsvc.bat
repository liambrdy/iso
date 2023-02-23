@echo off

set IGNORES=/Wv:18
set CFLAGS=/W4 /std:c11 /wd4996 /wd5105 /FC /TC /Zi /nologo %IGNORES%
set INCLUDES=/I dependencies\SDL2\include /I dependencies\GLEW\include
set LIBS=dependencies\SDL2\lib\x64\SDL2.lib ^
         dependencies\SDL2\lib\x64\SDL2main.lib ^
         dependencies\GLEW\lib\glew32s.lib ^
         opengl32.lib User32.lib Gdi32.lib Shell32.lib
set SRC_FILES=src\main.c src\vec.c src\renderer.c src\common.c

cl.exe %CFLAGS% %INCLUDES% %SRC_FILES% /Fobuild\ /Febuild\iso /link %LIBS% -SUBSYSTEM:console
