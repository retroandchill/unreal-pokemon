Get-ChildItem -Recurse . -Include *.cpp, *.h -Exclude "**/Deps/**" -ErrorAction SilentlyContinue -Force | ForEach-Object { 
    clang-format -i $_ 
}