# Get the first argument
$build_type = $args[0]

# If no argument is provided, default to Release
if (-not $build_type) {
    $build_type = "Release"
} elseif ($build_type -ne "Release" -and $build_type -ne "Debug") {
    throw "Invalid build type. Use 'Release' or 'Debug'"
}

$CURRENT_DIR = Get-Location
$PROJ_DIR = "$CURRENT_DIR/exp02-openmp"

## [Optional] Remove the old build
# Remove-Item -Recurse -Force "$PROJ_DIR/build"

# Create the build directory if it doesn't exist
if (-not (Test-Path "$PROJ_DIR/build")) {
    New-Item -ItemType Directory -Path "$PROJ_DIR/build"
}

Set-Location "$PROJ_DIR/build"

cmake .. -DCMAKE_BUILD_TYPE="${build_type}" -G "Ninja"

ninja

Set-Location -Path $CURRENT_DIR

Write-Host "[SHU-CAE] >>> Exp02 build finished." -ForegroundColor Green