#!/usr/bin/env pwsh

# ensure we have a valid set of arguments
if ($args.Count -lt 3) {
    Write-Error 'Expected 3 arguments'
    exit 1 # failed to run script
}

# prepare the target details to be used
$target_dir = $args[2]
$target_arch = $args[1]
$target_system = $args[0]
$target_release = $args[3]

# prepare the incoming target cache file
$target_cache = "$target_dir/CMakeCache.txt"

# prepare whether or not we should run a post-build at all
$target_post = if ($target_dir -eq "build") { "ON" } else { "OFF" }
$target_canary = if ($target_release -eq "stable") { "OFF" } else { "ON" }
$target_toolchain = "cmake/mono/toolchains/$target_system-$target_arch.cmake"

# ensure we have a directory rebuilt from scratch
New-Item -ItemType Directory -Path $target_dir -Force | Out-Null

# remove the previous cache (just-in-case)
if (Test-Path -Path $target_cache -PathType Leaf) {
    Remove-Item -Path $target_cache -Force | Out-Null
}

# configure the cmake project we require now
cmake -S (Get-Location).Path -B $target_dir -G Ninja `
    -DSABRE_OPTION_CANARY:BOOL=$target_canary `
    -DSABRE_OPTION_POSTBUILD:BOOL=$target_post `
    -DFETCHCONTENT_BASE_DIR="build/_deps" `
    -DCMAKE_BUILD_TYPE:STRING=Release `
    -DCMAKE_TOOLCHAIN_FILE:FILEPATH=$target_toolchain

# attempt building the cmake project safely now
cmake --build $target_dir --config Release --target all --parallel
