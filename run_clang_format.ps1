Get-ChildItem -Recurse . -Include *.cpp, *.h -Exclude "**/Deps/**,**/External/**" -ErrorAction SilentlyContinue -Force | ForEach-Object {
    clang-format -i $_
}

git submodule foreach --recursive git reset --hard