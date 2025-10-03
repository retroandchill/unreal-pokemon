import os
import subprocess
from argparse import ArgumentParser
from dataclasses import dataclass
from typing import NamedTuple, TypeAlias, Callable

import pathspec
from pqdm.threads import pqdm


# Define strongly typed structures
@dataclass
class FormatResult:
    success: bool
    filepath: str
    message: str


class FormatArgs(NamedTuple):
    dirpath: str
    filename: str
    check_only: bool


# Define type aliases for our callbacks
FormatCallback: TypeAlias = Callable[[FormatArgs], FormatResult]
ErrorCallback: TypeAlias = Callable[[Exception], None]


def load_ignore_patterns(ignore_file='.clang-format-ignore'):
    try:
        with open(ignore_file, 'r', encoding='utf-8-sig') as f:
            patterns = [line.encode('ascii', 'ignore').decode('ascii').strip()
                        for line in f if line.strip() and not line.startswith('#')]
            spec = pathspec.PathSpec.from_lines(
                pathspec.patterns.GitWildMatchPattern, patterns
            )
            print("Loaded ignore patterns:", patterns)  # Debug print
            return spec
    except FileNotFoundError:
        # Default patterns if no ignore file exists
        patterns = []
        return pathspec.PathSpec.from_lines(
            pathspec.patterns.GitWildMatchPattern, patterns
        )


def find_files(spec: pathspec.PathSpec, check_only: bool) -> list[FormatArgs]:
    """
    Returns list of tuples (dirpath, filename) for files that need formatting
    """
    LINE_ENDINGS = {'.h', '.cpp', '.hpp', '.c', '.hxx', '.cxx'}
    files_to_format: list[FormatArgs] = []

    for dirpath, dirnames, filenames in os.walk('.'):
        # Get relative path for matching
        rel_path = os.path.relpath(dirpath, '.')

        # Skip if directory matches ignore patterns
        if spec.match_file(rel_path):  # type: ignore[type-arg]
            dirnames.clear()  # Skip all subdirectories
            continue

        for filename in filenames:
            if not any(filename.endswith(ext) for ext in LINE_ENDINGS):
                continue

            full_path = os.path.join(
                rel_path, filename)  # type: ignore[type-arg]
            if not spec.match_file(full_path):  # type: ignore[type-arg]
                files_to_format.append(
                    FormatArgs(
                        dirpath,
                        filename,
                        check_only))

    return files_to_format


def format_file(args: FormatArgs) -> FormatResult:
    """
    Format a single file and return success status and filename
    """
    filepath = os.path.join(args.dirpath, args.filename)
    try:
        if args.check_only:
            result = subprocess.run(
                ['clang-format', filepath],
                capture_output=True,
                text=True,
                check=True
            )

            # Compare the output with the current file content
            with open(filepath, 'r') as f:
                current_content = f.read()

            if current_content != result.stdout:
                print(f"File {filepath} needs formatting")

            needs_formatting = current_content != result.stdout

            return FormatResult(
                success=not needs_formatting,
                filepath=filepath,
                message="" if not needs_formatting else "needs formatting"
            )
        else:
            subprocess.run(['clang-format', '-i', filepath],
                           check=True, capture_output=True)
            return FormatResult(
                success=True,
                filepath=filepath,
                message=""
            )
    except Exception as e:
        return FormatResult(success=False, filepath=filepath, message=str(e))


def main(check: bool):
    # Load ignore patterns
    spec = load_ignore_patterns()

    # Find all files that need formatting
    print("Scanning for files...")
    files_to_format = find_files(spec, check)
    total_files = len(files_to_format)
    print(f"Found {total_files} files to format")

    if not total_files:
        print("No files to format")
        return

    # Determine optimal number of workers based on CPU cores
    max_workers = min(32, os.cpu_count() * 2)  # Limit max threads

    print(f"Starting format with {max_workers} workers...")
    results: list[FormatResult] = pqdm(
        files_to_format, format_file, n_jobs=max_workers)

    errors = [r.message for r in results if not r.success]
    error_count = len(errors)

    if error_count:
        print(f"Errors encountered: {error_count} files")
        print("\nErrors:")
        for error in errors:
            print(error)

        return False

    return True


if __name__ == '__main__':
    # Add command line argument parsing
    parser = ArgumentParser(description='Format C++ files using clang-format')
    parser.add_argument('--check', action='store_true',
                        help='Check if files need formatting without modifying them', default=False)
    args = parser.parse_args()

    success = main(args.check)
    exit(0 if success else 1)
