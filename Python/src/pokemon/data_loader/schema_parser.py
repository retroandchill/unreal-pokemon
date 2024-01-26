from enum import Enum
from typing import Type, Optional, TypeVar
from warnings import warn
import re


def convert_data_to_json(section_name: str, data: dict[str, str], schema: dict[str, tuple[str, Optional[Type[Enum]]]]) -> dict[str, any]:
    ret = {'ID': string_to_json_value(section_name, schema["SectionName"][0])}
    for key, value in data.items():
        s = schema[key]
        ret[key] = parse_csv_value(value, s[0], s[1])

    return ret


def parse_csv_value(value: str, schema: str, enumeration: Optional[dict[str, str]]):
    ret = []
    repeat = False
    start = 0
    schema_length = len(schema)
    match schema[0]:
        case '*':
            repeat = True
            start = 1
        case '^':
            start = 1
            schema_length -= 1

    subarrays = repeat and len(schema) - start > 1
    regex = re.compile(r",(?=(?:[^\"']*[\"'][^\"']*[\"'])*[^\"']*$)")
    values = regex.split(value)
    idx = -1
    while True:
        idx, record = method_name(idx, value, values, schema, start, enumeration)

        if record:
            if subarrays:
                ret.append(record)
            else:
                ret.extend(record)

        if not repeat or idx >= len(values) - 1:
            break

    return ret[0] if not repeat and schema_length == 1 else ret


def method_name(idx: int, value: any, values: list[any], schema: str, start: int, enumeration: Optional[dict[str, str]]):
    record = []
    for i in range(start, len(schema)):
        idx += 1
        sche = schema[i]
        if re.match(r'[A-Z]', sche) and not values[idx]:
            record.append(None)
            continue

        if sche.lower() == 'q':
            record.append(value)
            idx = len(values)
            break
        else:
            record.append(string_to_json_value(values[idx], sche, enumeration))
    return idx, record


def string_to_json_value(value: str, schema: str, enumeration: Optional[Type[Enum]] = None):
    match schema.lower():
        case 'i':
            return int(value)
        case 'u':
            ret = int(value)
            if ret < 0:
                raise ValueError('Field {0} is not a positive integer or 0.'.format(value))

            return ret
        case 'v':
            ret = int(value)
            if ret <= 0:
                raise ValueError('Field {0} is not a positive integer.'.format(value))

            return ret
        case 'x':
            return int(value, 16)
        case 'f':
            return float(value)
        case 'b':
            return bool(value)
        case 'n':
            if re.match(r'^(?!\d)\w+$', value):
                raise ValueError("Field '{0}' must contain only letters, digits, and underscores and can't "
                                 "begin with a number.".format(value))

            return value
        case 's':
            return value
        case 'q':
            return value
        case 'm':
            return value
        case 'e':
            return check_enum_field(value, enumeration)
        case 'y':
            if re.match(r'-?\d+$', value):
                return int(value)

            return check_enum_field(value, enumeration)

    raise ValueError("Failed to parse field '{0}' with provided schema value '{1}'".format(value, schema))


def check_enum_field(field: str, enumeration: Optional[set[str]]) -> str:
    if enumeration is None:
        warn("Field '{0}' was specified as an enumeration, but no enumeration was provided! "
             "Validation will be skipped".format(field))
    elif field not in enumeration:
        raise ValueError("Undefined value {0} in {1}.".format(field, str(enumeration)))

    return field
