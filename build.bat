@echo off
setlocal

:: ================================
:: MazeGame one-command build script
:: ================================

:: Build configuration (Debug / Release / RelWithDebInfo)
set CONFIG=Release

:: Build directory
set BUILD_DIR=build

echo === MazeGame Build (%CONFIG%) ===

:: Ensure MSVC environment is available
where cl >nul 2>nul
if errorlevel 1 (
    echo [INFO] MSVC not found. Initializing Visual Studio Build Tools...
    call "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" ^
        -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 ^
        -property installationPath > "%TEMP%\vs_path.txt"

    set /p VS_PATH=<"%TEMP%\vs_path.txt"
    call "%VS_PATH%\VC\Auxiliary\Build\vcvars64.bat"
)

:: Configure
cmake -S . -B %BUILD_DIR% -G Ninja -DCMAKE_BUILD_TYPE=%CONFIG%
if errorlevel 1 goto error

:: Build
cmake --build %BUILD_DIR%
if errorlevel 1 goto error

echo.
echo === Build completed successfully ===
echo Executable: %BUILD_DIR%\MazeGame.exe
exit /b 0

:error
echo.
echo === BUILD FAILED ===
exit /b 1
