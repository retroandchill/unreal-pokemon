Get-ChildItem -Recurse . -Include *.cpp, *.h -ErrorAction SilentlyContinue -Force | ForEach-Object { 
    & "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\x64\bin\clang-format.exe" -i -style=Google $_ 
}