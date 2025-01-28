$location = $( Get-Location )
echo "Location: $location"
$unreal_engine_location = "C:/Program Files/Epic Games/UE_5.5/Engine/Binaries/Win64"
OpenCppCoverage --modules ${location}\**\Binaries\Win64 --sources ${location}\Source\**\* --sources ${location}\Plugins\**\* --excluded_sources .gen. --excluded_sources \PokemonTests\ --export_type cobertura:cpp-coverage.xml --export_type html:codecoverage/Report --optimized_build --excluded_line_regex ".*\(.*\)\s*(?:noexcept)?\s*=\s*default;.*" --excluded_line_regex ".*GENERATED_BODY\(\).*" -- "${unreal_engine_location}/UnrealEditor-Win64-DebugGame-Cmd.exe" "${location}/UnrealPokemon.uproject" -clientconfig=DebugGame -nosplash -unattended -RenderOffscreen -execcmds="automation runtests Unit Tests+Functional Tests+Python.UnrealPokemon,ShutdownAfterTests" -log -ReportOutputPath="${location}/TestResults"
if ($LASTEXITCODE -ne 0) {
    echo "Tests failed with exit code: ${LASTEXITCODE}"
    exit -1
}
python ./ci/verify_test_success.py --file TestResults/index.json
if ($LASTEXITCODE -ne 0) {
    exit -1
}