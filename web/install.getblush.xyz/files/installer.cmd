@echo off
setlocal enabledelayedexpansion

echo Choose your language / Wybierz język / Scegli la lingua / Choisissez la langue / Elige idioma / Vyberte jazyk:
echo 1) English
echo 2) Polish
echo 3) Italian
echo 4) French
echo 5) Spanish
echo 6) Czech
set /p choice=Enter number: 

if "%choice%"=="1" (
    set MSG_DL=Downloading Blush...
    set MSG_OK=Blush installed and added to PATH!
) else if "%choice%"=="2" (
    set MSG_DL=Pobieranie Blusha...
    set MSG_OK=Blush został zainstalowany i dodany do PATH!
) else if "%choice%"=="3" (
    set MSG_DL=Scaricando Blush...
    set MSG_OK=Blush installato e aggiunto al PATH!
) else if "%choice%"=="4" (
    set MSG_DL=Téléchargement de Blush...
    set MSG_OK=Blush installé et ajouté au PATH!
) else if "%choice%"=="5" (
    set MSG_DL=Descargando Blush...
    set MSG_OK=Blush instalado y agregado al PATH!
) else if "%choice%"=="6" (
    set MSG_DL=Stahování Blush...
    set MSG_OK=Blush nainstalován a přidán do PATH!
) else (
    set MSG_DL=Downloading Blush...
    set MSG_OK=Blush installed and added to PATH!
)

set ARCH=%PROCESSOR_ARCHITECTURE%
if /i "%ARCH%"=="AMD64" set BLUSH_ARCH=x64
if /i "%ARCH%"=="ARM64" set BLUSH_ARCH=arm64
if not defined BLUSH_ARCH (
    echo No binary available for %ARCH%
    exit /b 1
)

set BLUSHDIR=%LOCALAPPDATA%\.blush
if not exist "%BLUSHDIR%" mkdir "%BLUSHDIR%"
set OUTPUT=%BLUSHDIR%\blush.exe

set URL=https://cdn.getblush.xyz/latest/blush-win-%BLUSH_ARCH%.exe

echo %MSG_DL%
echo Downloading from: %URL%
echo Will save to: %OUTPUT%

bitsadmin /transfer BlushDownloadJob /download /priority normal "%URL%" "%OUTPUT%"

for %%F in ("%OUTPUT%") do set SIZE=%%~zF
if %SIZE% LSS 512000 (
    echo Download failed or file too small (<500KB)
    exit /b 1
)

reg query "HKCU\Environment" /v PATH | find /i "%BLUSHDIR%" >nul
if errorlevel 1 (
    setx PATH "%BLUSHDIR%;%PATH%"
)

echo %MSG_OK%
echo Installed at: %OUTPUT%
echo You may need to restart CMD to apply PATH changes.
pause
