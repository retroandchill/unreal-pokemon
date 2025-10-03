import subprocess
import sys
from argparse import ArgumentParser

from run_clang_format import main as run_clang_format


def main():
    # Add command line argument parsing
    parser = ArgumentParser(description='Format C++ files using clang-format')
    parser.add_argument('--check', action='store_true',
                        help='Check if files need formatting without modifying them', default=False)
    args = parser.parse_args()

    check: bool = args.check

    failed = False
    # Run clang-format directly by calling its main function
    print("Running clang-format...")
    if not run_clang_format(check):
        failed = True

    # Run csharpier (still needs to be a subprocess)
    print("\nRunning csharpier...")
    if check:
        result = subprocess.run(["csharpier", "check", "."], check=False)
    else:
        result = subprocess.run(["csharpier", "format", "."], check=False)
    if result.returncode != 0:
        failed = True

    exit(1 if failed else 0)


if __name__ == "__main__":
    main()
