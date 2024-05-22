# Get the first argument
$build_type = $args[0]

# If no argument is provided, default to Release
if (-not $build_type) {
    $build_type = "Release"
} elseif ($build_type -ne "Release" -and $build_type -ne "Debug") {
    throw "Invalid build type. Use 'Release' or 'Debug'"
}

$PROJ_DIR = Get-Location
$Yutils_DIR = "$PROJ_DIR/vendor/Yutils"

## [Optional] Remove the old build
# Remove-Item -Recurse -Force "$Yutils_DIR/build"

# Create the build directory if it doesn't exist
if (-not (Test-Path "$Yutils_DIR/build")) {
    New-Item -ItemType Directory -Path "$Yutils_DIR/build"
}

Set-Location "$Yutils_DIR/build"

cmake .. -DCMAKE_BUILD_TYPE="${build_type}" -G "Ninja"

ninja

Set-Location -Path $PROJ_DIR

Write-Host "Build finished successfully."