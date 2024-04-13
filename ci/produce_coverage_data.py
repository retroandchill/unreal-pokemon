import glob
import os.path
import re
import subprocess
import sys


def filter_tests_and_generated(path: str):
    if re.match(r'.*PokemonTests.*', path):
        return False
    else:
        return True


def get_target_dir(build_file: str) -> str:
    return os.path.dirname(build_file)


if __name__ == "__main__":
    llvm_dir = sys.argv[1]
    profdata_file = sys.argv[2]

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

    cmd = [f"{llvm_dir}/llvm-cov.exe", "show", f"--instr-profile={profdata_file}"]
    cmd += binary_files
    cmd.append("-sources")
    cmd += source_targets

    with open("coverage.txt", "w") as file:
        subprocess.run(cmd, stdout=file, stderr=file)
