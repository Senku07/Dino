@echo off
md build
pushd build
cl ../src/main.cpp /Zi  /Fe:Dino.exe /I"../dp" /MD /link user32.lib opengl32.lib shell32.lib gdi32.lib kernel32.lib ../dp/glfw3.lib /nologo
if %ERRORLEVEL% EQU 0 (
    Dino.exe
    cls
)
popd