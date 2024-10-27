import json
from argparse import ArgumentParser

if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument('-f', '--file', type=str)

    args = parser.parse_args()
    try:
        with open(args.file, 'r') as file:
            test_report = json.load(file)
    except IOError:
        print(f'Failed to open file: {args.file}')
        exit(1)

    if test_report['failed'] > 0:
        print(f'{test_report["failed"]} tests failed!')
        exit(1)
    else:
        print('All tests passed!')

