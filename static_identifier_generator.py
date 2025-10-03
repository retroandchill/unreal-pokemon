import json

FILE = 'Evolution'


def main():
    with open('output.txt', 'w') as fout:

        with open(f'./PBS/HardCodedJsonFiles/{FILE}s.json', 'r') as file:
            json_string = json.load(file)

        for item in json_string:
            if item['ID'] == 'None':
                continue

            print(f'"{item['ID']}",', file=fout)

        print(file=fout)
        for item in json_string:
            print(f'manager.{FILE}s.RegisterEntry(', file=fout)
            print(f'    new F{FILE}', file=fout)
            print('    {', file=fout)
            my_id = None
            for key, value in item.items():
                if key == 'ID':
                    if my_id == 'None':
                        continue

                    my_id = value
                    print(f'        ID = F{FILE}.{my_id},', file=fout)
                elif key == 'Name':
                    continue
                elif key == 'RealName':
                    print(
                        f'        Name = FText.Localized(LocalizationNamespace, "{my_id}", "{value}"),',
                        file=fout)
                elif isinstance(value, str):
                    print(f'        {key} = "{value}",', file=fout)
                else:
                    print(f'        {key} = {value},', file=fout)
            print('    }', file=fout)
            print(');', file=fout)
            print(file=fout)


if __name__ == "__main__":
    main()
