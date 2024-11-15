import argparse
import copy
import json
import os
import re

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-f', '--file', type=str)

    args = parser.parse_args()
    with open(args.file) as file:
        json_string = file.read()
        json_string = re.sub(r'^#(.*)\n?', '', json_string, flags=re.MULTILINE)
        build_wrapper_json = json.loads(json_string)

    captures = []
    for capture in build_wrapper_json['captures']:
        cmd: list[str] = capture['cmd']
        file_name = cmd[1]
        if not re.match(r'.*Module\.\w+\.cpp', file_name):
            captures.append(capture)
            continue

        with open(file_name) as file:
            file_string = file.read()
            matches = re.findall(r'#include "(.*)"', file_string)

            for include in matches:
                new_capture = copy.deepcopy(capture)
                capture['cmd'][1] = os.path.normpath(include)
                captures.append(new_capture)
                print(capture['cmd'][1])

    build_wrapper_json['captures'] = captures
    with open(args.file, 'w') as file:
        file.write(json.dumps(build_wrapper_json, indent=4))
