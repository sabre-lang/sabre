#!/usr/bin/env bash

# prepare a helper for failures
error() {
    echo -e "$@" >&2
    exit 1
}

# ensure we have a valid set of arguments
if [ $# -lt 3 ]; then
    error 'Expected 3 arguments'
fi

# prepare the target details to be used
target_dir=$3
target_arch=$2
target_system=$1
target_release=$4

# ensure we have a directory rebuilt from scratch
mkdir -p $target_dir

# prepare the incoming target cache file
target_cache="$target_dir/CMakeCache.txt"

# determine if using a post-build response
target_post=$(if [ $target_dir = "build" ]; then echo "ON"; else echo "OFF"; fi)
target_canary=$(if [ $target_release = "stable" ]; then echo "OFF"; else echo "ON"; fi)
target_toolchain="cmake/mono/toolchains/$target_system-$target_arch.cmake"

# remove the incoming cache file now as well
rm -f $target_cache

# configure the cmake project we require now
cmake -S $(pwd) -B $target_dir -G Ninja \
    -DSABRE_OPTION_CANARY:BOOL=$target_canary \
    -DSABRE_OPTION_POSTBUILD:BOOL=$target_post \
    -DFETCHCONTENT_BASE_DIR="build/_deps" \
    -DCMAKE_BUILD_TYPE:STRING=Release \
    -DCMAKE_TOOLCHAIN_FILE:FILEPATH=$target_toolchain

# attempt building the cmake project safely now
cmake --build $target_dir --config Release --target all --parallel
