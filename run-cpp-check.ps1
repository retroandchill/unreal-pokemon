$engine_dir = "C:/Program Files/Epic Games/UE_5.3/Engine/Source"

$include_dirs = ""

Get-ChildItem $engine_dir Public -Recurse -Directory -Name | ForEach-Object {
    $include_dirs += "-I ${_} "
}

cppcheck --cppcheck-build-dir=build $include_dirs Source