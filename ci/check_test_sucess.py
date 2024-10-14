import json
import sys

if __name__ == "__main__":
    output_log = sys.argv[1]

    with open(output_log, 'r', encoding='utf-8-sig') as f:
        data = json.load(f)

    if 'failed' in data:
        print(data['failed'])
    else:
        print(-1)
