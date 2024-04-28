$location = $( Get-Location )
$unreal_engine_location = "C:/Program Files/Epic Games/UE_5.4/Engine/Binaries/Win64"
& "${unreal_engine_location}/UnrealEditor-Win64-DebugGame-Cmd.exe" "${location}/UnrealPokemon.uproject" -clientconfig=DebugGame -nosplash -unattended -nullrhi -execcmds="automation runtests Unit Tests+Functional Tests+Python.UnrealPokemon,ShutdownAfterTests" -log -ReportOutputPath="${$location}/TestResults"
Copy-Item "${unreal_engine_location}/default.profraw" "${location}/default.profraw"

$clang_cl_location = "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/Llvm/x64/bin/"
& "${clang_cl_location}/llvm-profdata.exe" merge --output=unreal.profdata *.profraw
python ./ci/produce_coverage_data.py "${clang_cl_location}" unreal.profdata