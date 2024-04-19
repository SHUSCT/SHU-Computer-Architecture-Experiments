# This script sets the environment variables for the Visual Studio 2022 Community Edition
# and the Windows 10 SDK. The script is intended to be run in a PowerShell terminal.

# >>>>>> [WARNING] >>>>>>
# 1. This script is provided as-is and is intended for educational purposes only.
# 2. Backup your existing environment variables before running this script.
# 3. Make sure to replace the paths with the actual paths on your system.
# 4. Comment out some variables if they already exist in the environment.
$MSVC = 'C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.39.33519'
$WINKIT10_INCLUDE = 'C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0'
$WINKIT10_LIB = 'C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0'
$WINKIT10_BIN = 'C:\Program Files (x86)\Windows Kits\10\bin\10.0.22621.0'
$CMAKE_BIN = 'C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin'
$NINJA_BIN = 'C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja'
# <<<<<< [WARNING] <<<<<<

# Ask the user to confirm before proceeding
Write-Host "Please check the paths in the script before running it."
$confirmation = Read-Host -Prompt "[DANGER] Are you sure to update the environment variables? (y/N)"

if ($confirmation -ne 'y' -and $confirmation -ne 'Y') {
    Write-Host "Exiting the script..."
    exit
}

# Update the INCLUDE environment variable
$currentInclude = [System.Environment]::GetEnvironmentVariable('INCLUDE', [System.EnvironmentVariableTarget]::User)
Write-Host "Current INCLUDE environment variable: `n" `
    $currentInclude -ForegroundColor Yellow
$confirmUpdateInclude = Read-Host -Prompt "Do you want to update the INCLUDE environment variable? (y/N)"
if ($confirmUpdateInclude -eq 'y' -or $confirmUpdateInclude -eq 'Y') {
    $updatedInclude = "$currentInclude;$WINKIT10_INCLUDE\ucrt;$WINKIT10_INCLUDE\um;$WINKIT10_INCLUDE\shared;$MSVC\include;"
    [System.Environment]::SetEnvironmentVariable('INCLUDE', $updatedInclude, [System.EnvironmentVariableTarget]::User)
}

# Update the LIB environment variable
$currentLib = [System.Environment]::GetEnvironmentVariable('LIB', [System.EnvironmentVariableTarget]::User)
Write-Host "Current LIB environment variable: `n" `
    $currentLib -ForegroundColor Yellow
$confirmUpdateLib = Read-Host -Prompt "Do you want to update the LIB environment variable? (y/N)"
if ($confirmUpdateLib -eq 'y' -or $confirmUpdateLib -eq 'Y') {
    $updatedLib = "$currentLib;$WINKIT10_LIB\um\x64;$WINKIT10_LIB\ucrt\x64;$MSVC\lib\x64;"
    [System.Environment]::SetEnvironmentVariable('LIB', $updatedLib, [System.EnvironmentVariableTarget]::User)
}

# Update the PATH environment variable
$currentPath = [System.Environment]::GetEnvironmentVariable('PATH', [System.EnvironmentVariableTarget]::User)
Write-Host "Current PATH environment variable: `n" `
    $currentPath -ForegroundColor Yellow
$confirmUpdatePath = Read-Host -Prompt "Do you want to update the PATH environment variable? (y/N)"
if ($confirmUpdatePath -ne 'y' -and $confirmUpdatePath -ne 'Y') {
    Write-Host "Exiting the script..."
    exit
}
$updatedPath = "$currentPath;$MSVC\bin\Hostx64\x64;$WINKIT10_BIN\x64;$CMAKE_BIN;$NINJA_BIN;"
[System.Environment]::SetEnvironmentVariable('PATH', $updatedPath, [System.EnvironmentVariableTarget]::User)

Write-Host "Environment variables updated successfully. Remember to restart the terminal."