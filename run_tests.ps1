$location = $( Get-Location )
OpenCppCoverage --modules ${location}\Binaries\Win64 --sources ${location}\Source\**\* --excluded_sources .gen. --excluded_sources \Tests\ --export_type cobertura:coverage.xml -- "C:\Program Files\Epic Games\UE_5.3\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" ${location}\UnrealPokemon.uproject -nosplash -unattended -nullrhi -nosound -execcmds="automation runtests Project;quit"^ -TestExit="Automation Test Queue Empty"