from enum import Enum
from typing import Type, Optional, TypeVar
from warnings import warn
import re


def convert_data_to_json(section_name: str, data: dict[str, str], schema: dict[str, tuple[str, Optional[Type[Enum]]]]) -> dict[str, any]:
    ret = {'ID': string_to_json_value(section_name, schema["SectionName"][0])}
    for key, value in data.items():
        s = schema[key]
        ret[key] = string_to_json_value(value, s[0], s[1])

    return ret


def string_to_json_value(value: str, schema: str, enumeration: Optional[Type[Enum]] = None):
    match schema:
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

    if field not in enumeration:
        raise ValueError("Undefined value {0} in {1}.".format(field, str(enumeration)))

    return field
