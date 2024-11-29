import os
import subprocess

STAY_OUT = ['cmake-build-debug', 'cmake-build-release', 'cmake-build-relwithdebinfo', 'cmake-build-minsize', '.venv']
LINE_ENDINGS = ['.cs', '.h', '.cpp', '.hpp', '.c', '.hxx', '.cxx', '.ixx']

if __name__ == '__main__':

    for dirpath, dirnames, filenames in os.walk('.'):
        if any(d in dirpath for d in STAY_OUT):
            continue

        for filename in filenames:
            if any(filename.endswith(e) for e in LINE_ENDINGS):
                subprocess.run(['clang-format', '-i', os.path.join(dirpath, filename)])