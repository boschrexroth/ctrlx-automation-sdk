#!/usr/bin/env bash

mddb_compiler -in metadata.csv -out metadata.mddb

ARCH=x86_64
CONFIG=Debug

for arg in "$@"
do
  if grep -q "ar" <<<${arg}; then
    ARCH=aarch64
  fi

  if grep -q "re" <<<${arg}; then
    CONFIG=Release
  fi

  if grep -q "help" <<<${arg}; then
    echo "Build (one) executable of a cpp project"
    echo "Parameters:"
    echo "   CPU Architecture: amd64 (default) | arm64 | aarch64"
    echo "   Configuration: debug (default) | release"
    echo "   Search pattern in snap/snapcraft: command (default) | source"
    echo "   Help (this info):  help"
    exit 1
  fi

done

# The name of the (first) executable (here e.g. xxx) will be read from snap/snapcraft.yaml
# 
# apps:
#   xxx:
#     command: xxx
# 
# Result of grep is: '    command: xxx'
# Result of cut is: ' xxx'
# Result of xargs is: 'xxx'
EXE=$(grep command: snap/snapcraft.yaml | cut -d':' -f2 | xargs)

if test -z "${EXE}" 
then
      echo "ERROR: snap/snapcraft.yaml doesn't contain 'apps: ... command:'"
	  echo ${EXE}
	  exit 1
fi

echo -------------------------------------------------------------------
echo Architecture: ${ARCH}
echo Configuration: ${CONFIG}
echo Executable: ${EXE}
echo -------------------------------------------------------------------

rm -rf generated/

cmake --no-warn-unused-cli \
    -DCMAKE_BUILD_KIT=${ARCH} \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_BUILD_TYPE:STRING=${CONFIG} \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/${ARCH}-linux-gnu-gcc-9 \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/${ARCH}-linux-gnu-g++-9 \
	-H/$(pwd) \
	-B/$(pwd)/build \
    -G "Unix Makefiles"

cd $(pwd)/build
cmake --build . --config ${CONFIG} --target ${EXE} -j 6 --