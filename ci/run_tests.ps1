$location = $( Get-Location )
OpenCppCoverage --modules ${location}\Binaries\Win64 --sources ${location}\Source\**\* --excluded_sources .gen. --excluded_sources \PokemonTests\ --export_type cobertura:cpp-coverage.xml --export_type html:codecoverage/Report --optimized_build --excluded_line_regex ".*\(.*\)\s*(?:noexcept)?\s*=\s*default;.*" --excluded_line_regex ".*GENERATED_BODY\(\).*" -- "C:\Program Files\Epic Games\UE_5.4\Engine\Binaries\Win64\UnrealEditor-Win64-DebugGame-Cmd.exe" ${location}\UnrealPokemon.uproject -clientconfig=DebugGame -nosplash -unattended -nullrhi -execcmds="automation runtests UnrealPokemon;quit" -log -TestExit="Automation Test Queue Empty" -ReportOutputPath="TestResults"