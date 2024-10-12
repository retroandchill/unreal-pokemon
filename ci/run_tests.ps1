$location = $( Get-Location )
$unreal_engine_location = "C:/Program Files/Epic Games/UE_5.4/Engine/Binaries/Win64"
OpenCppCoverage --modules ${location}\**\Binaries\Win64 --sources ${location}\Source\**\* --sources ${location}\Plugins\**\* --excluded_sources .gen. --excluded_sources **\External\** --excluded_sources **\Deps\** --excluded_sources .gen. --export_type cobertura:cpp-coverage.xml --export_type html:codecoverage/Report --optimized_build --excluded_line_regex ".*\(.*\)\s*(?:noexcept)?\s*=\s*default;.*" --excluded_line_regex ".*GENERATED_BODY\(\).*" -- "${unreal_engine_location}/UnrealEditor-Win64-DebugGame-Cmd.exe" "${location}/UnrealPokemon.uproject" -clientconfig=DebugGame -nosplash -unattended -nullrhi -execcmds="automation runtests Unit Tests+Functional Tests+Python.UnrealPokemon,ShutdownAfterTests" -log -ReportOutputPath="${$location}/TestResults"
if ($LASTEXITCODE -ne 0) {
    exit -1
}