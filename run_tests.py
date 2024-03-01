
import os
import subprocess
from itertools import chain


def is_valid_cpp_file(name: str) -> bool:
    return any([name.endswith(ext) for ext in ['.h', '.cpp', '.hpp', '.c', '.hxx', '.cxx']])


if __name__ == '__main__':
    current_dir = os.path.dirname(__file__)
    os.chdir(current_dir)
    
    modules = [name for name in os.listdir('Source') if os.path.isdir(os.path.join('Source', name))]
    source_files = [os.path.join(dp, f) for dp, dn, fn in os.walk('Source') for f in fn if is_valid_cpp_file(f)]
    
    cmd = ['OpenCppCoverage']
    module_pairs = [['--modules', mod] for mod in modules]
    cmd += [item for items in module_pairs for item in items]
    source_pairs = [['--sources', src] for src in source_files]
    cmd += [item for items in source_pairs for item in items]
    project_name = os.path.join(current_dir, 'UnrealPokemon.exe')
    cmd += ['--export_type', 'html:code-coverage/report', '--', 
            'C:/Program Files/Epic Games/UE_5.3/Engine/Binaries/Win64/UnrealEditor.exe', 
            f'{project_name}']
    subprocess.run(cmd)
