Get-ChildItem -Recurse . -Include *.cpp, *.h -ErrorAction SilentlyContinue -Force | ForEach-Object { 
    clang-format -i $_ 
}