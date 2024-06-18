@echo off

set MBT_VERSION=0.9.0
set MBT=env\bin\MaterialBinTool-%MBT_VERSION%-native-image.exe
set SHADERC=env\bin\shaderc.exe
set DATA_DIR=data

set MBT_RELEASE_URL=https://github.com/devendrn/newb-shader/releases/download/dev/MaterialBinTool-$MBT_VERSION-all.jar
set M_DATA_URL=https://github.com/devendrn/RenderDragonData

if not exist %MBT% (
  mkdir env\bin\
  echo Downloading MaterialBinTool-%MBT_VERSION%-native-image.exe
  curl -L -o %MBT% %MBT_RELEASE_URL%/MaterialBinTool-%MBT_VERSION%-native-image.exe
)

if not exist %SHADERC% (
  echo Downloading shaderc.exe
  curl -L -o %SHADERC% %MBT_RELEASE_URL%/shaderc.exe
)

if not exist %DATA_DIR% (
  echo Cloning RenderDragonData
  git clone --filter=tree:0 %M_DATA_URL% %DATA_DIR%
) else (
  cd %DATA_DIR%
  git pull
)
