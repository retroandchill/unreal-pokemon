$location = $( Get-Location )
echo "Location: $location"
$unreal_engine_location = "C:/Program Files/Epic Games/UE_5.5/Engine/Binaries/Win64"
& "${unreal_engine_location}/UnrealEditor-Win64-DebugGame-Cmd.exe" "${location}/UnrealPokemon.uproject" -clientconfig=DebugGame -nosplash -unattended -nullrhi -execcmds="automation runtests Unit Tests+Functional Tests+Python.UnrealPokemon,ShutdownAfterTests" -log -ReportOutputPath="${location}/TestResults"
if ($LASTEXITCODE -ne 0) {
    echo "Tests failed with exit code: ${LASTEXITCODE}"
    exit -1
}
python ./ci/verify_test_success.py --file TestResults/index.json
if ($LASTEXITCODE -ne 0) {
    exit -1
}

Copy-Item "${unreal_engine_location}/default.profraw" "${location}/default.profraw"

$clang_cl_location = "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/Llvm/x64/bin/"
& "${clang_cl_location}/llvm-profdata.exe" merge --output=unreal.profdata *.profraw
python ./ci/produce_coverage_data.py "${clang_cl_location}" unreal.profdata