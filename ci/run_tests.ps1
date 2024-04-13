$location = $( Get-Location )
$unreal_engine_location = "C:/Program Files/Epic Games/UE_5.4/Engine/Binaries/Win64"
& "${unreal_engine_location}/UnrealEditor-Win64-DebugGame-Cmd.exe" "${location}/UnrealPokemon.uproject" -clientconfig=DebugGame -nosplash -unattended -nullrhi -execcmds="automation runtests UnrealPokemon.Math,FullShutdown" -log -ReportOutputPath="TestResults"
Copy-Item "${unreal_engine_location}/default.profraw" "${location}/default.profraw"