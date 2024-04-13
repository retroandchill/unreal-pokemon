import glob
import os.path
import re
import subprocess


def filter_tests_and_generated(path: str):
    if re.match(r'.*PokemonTests.*', path):
        return False
    else:
        return True


def get_target_dir(build_file: str) -> str:
    return os.path.dirname(build_file)


if __name__ == "__main__":
    binary_files = glob.glob("./**/UnrealEditor-*-Win64-DebugGame.dll", recursive=True)
    source_targets = list(map(get_target_dir,
                              list(filter(filter_tests_and_generated,
                                          glob.glob("./**/*.Build.cs", recursive=True)))))

    for i in range(len(binary_files)):
        # Insert the element after each existing element
        binary_files.insert(i * 2 + 1, "-object")
    binary_files.pop()

    for i in range(len(source_targets)):
        # Insert the element after each existing element
        source_targets.insert(i * 2 + 1, "-sources")
    source_targets.pop()

    cmd = ["C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/Llvm/x64/bin/llvm-cov.exe", "show",
           "--instr-profile=unreal.profdata"]
    cmd += binary_files
    cmd.append("-sources")
    cmd += source_targets

    with open("coverage.txt", "w") as file:
        subprocess.run(cmd, stdout=file, stderr=file)
