./ci/set-sonar-variables.ps1
$Location = $(Get-Location)
& $env:SONAR_DIRECTORY/build-wrapper-win-x86/build-wrapper-win-x86-64.exe --out-dir bw-output "C:\Program Files\Epic Games\UE_5.5\Engine\Binaries\DotNet\UnrealBuildTool\UnrealBuildTool.exe" "${Location}/UnrealPokemon.uproject" UnrealEditor Win64 DebugGame