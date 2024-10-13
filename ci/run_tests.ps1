$location = $( Get-Location )
$unreal_engine_location = "C:/Program Files/Epic Games/UE_5.4/Engine/Binaries/Win64"
echo "Running tests..."
& "${unreal_engine_location}/UnrealEditor-Win64-DebugGame-Cmd.exe" "${location}/UnrealPokemon.uproject" -clientconfig=DebugGame -nosplash -unattended -nullrhi -execcmds="automation runtests Unit Tests+Functional Tests+Python.UnrealPokemon,ShutdownAfterTests" -log -ReportOutputPath="${$location}/TestResults"
if ($LASTEXITCODE -ne 0) {
    echo "Tests failed with exit code: ${$LASTEXITCODE}"
    exit -1
}

echo "Tests Passed!"
Copy-Item "${unreal_engine_location}/default.profraw" "${location}/default.profraw"

echo "Outputing coverage data"
$clang_cl_location = "C:/Program Files/Llvm/bin/"
& "${clang_cl_location}/llvm-profdata.exe" merge --output=unreal.profdata *.profraw
python ./ci/produce_coverage_data.py "${clang_cl_location}" unreal.profdata