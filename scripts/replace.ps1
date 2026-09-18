#!/usr/bin/env pwsh

# -  PARAMETERS  - #

param(
    # Sabre Archive Source.
    [String]$Source,

    # Sabre Home Folder.
    [String]$Destination
);

# -  RUNNER  - #

# Replaces the desired destination with a source folder.
Remove-Item -Path "$Destination" -Recurse -Force -ErrorAction SilentlyContinue;
Move-Item -Path "$Source" -Destination "$Destination" -ErrorAction SilentlyContinue;
