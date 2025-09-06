import os
import subprocess

STAY_OUT = ['Deps', 'External', 'Intermediate', 'UnrealSharp/UnrealSharp']
LINE_ENDINGS = ['.h', '.cpp', '.hpp', '.c', '.hxx', '.cxx']

if __name__ == '__main__':

    for dirpath, dirnames, filenames in os.walk('.'):
        if any(d in dirpath for d in STAY_OUT):
            continue

        for filename in filenames:
            if any(filename.endswith(e) for e in LINE_ENDINGS):
                subprocess.run(['clang-format', '-i', os.path.join(dirpath, filename)])
