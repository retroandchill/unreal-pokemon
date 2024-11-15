import os
import subprocess

STAY_OUT = ['Deps', 'External', 'Intermediate']

if __name__ == '__main__':

    for dirpath, dirnames, filenames in os.walk('.'):
        if any(d in dirpath for d in STAY_OUT):
            continue

        for filename in filenames:
            if filename.endswith('.cpp') or filename.endswith('.h'):
                subprocess.run(['clang-format', '-i', os.path.join(dirpath, filename)])
