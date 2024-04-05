# "Unreal Pokémon" created by Retro & Chill.
import re
from typing import Optional
from warnings import warn

from pokemon.data_loader.unreal_data_loader import DataContainer


def convert_data_to_json(section_name: str, data: dict[str, str],
                         schema: dict[str, tuple[str, str, Optional[DataContainer]]]) -> dict[str, any]:
    """
    Converts a raw INI dictionary to one that can be converted to a JSON
    :param section_name: The name of the section to convert
    :param data: The unformatted data dictionary
    :param schema: The schema to convert the string data by
    :return: The formatted data dictionary
    """
    ret = {'Name': string_to_json_value(section_name, schema["SectionName"][1])}
    ret['ID'] = ret['Name']
    for key, value in data.items():
        s = schema[key]
        enums = []
        for i in range(2, len(s)):
            enums.append(s[i])

        ret[s[0]] = parse_ini_value(value, s[1], enums)

    return ret


def parse_ini_value(value: str, schema: str, enumerations: list[Optional[DataContainer]]) -> any:
    """
    Parses a string in in the INI file and returns whatever data the schema requires
    :param value: The string representation of the value
    :param schema: The schema for the data
    :param enumerations: The enumerations to validate the data against (only needed for enumeration fields)
    :return: The formmated data
    """
    ret = []
    repeat = False
    start = 0
    schema_length = len(schema)
    if schema[0] == '*':
        repeat = True
        start = 1
    elif schema[0] == '^':
        start = 1
        schema_length -= 1

    subarrays = repeat and len(schema) - start > 1
    regex = re.compile(r",(?=(?:[^\"']*[\"'][^\"']*[\"'])*[^\"']*$)")
    values = regex.split(value)
    idx = -1
    while True:
        idx, record = get_index_and_record(idx, value, values, schema, start, enumerations)

        if record:
            if subarrays:
                ret.append(record)
            else:
                ret.extend(record)

        if not repeat or idx >= len(values) - 1:
            break

    return ret[0] if not repeat and schema_length == 1 else ret


def get_index_and_record(idx: int, value: str, values: list[str], schema: str, start: int,
                         enumerations: list[Optional[dict[str, str]]]):
    """
    Get the index and record information for the given schema value
    :param idx: The current index at the beginning of this function
    :param value: The string representation of the value
    :param values: The full set of values for the given string
    :param schema: The schema to parse the value from
    :param start: The index to begin iteration from
    :param enumerations: The enumeration to validate the data against (only needed for enumeration fields)
    :return: A tuple containing the index following iteration and record information
    """
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
            enum = enumerations[i - start] if i - start < len(enumerations) else None
            record.append(string_to_json_value(values[idx], sche.lower(), enum))
    return idx, record


def string_to_json_value(value: str, schema: str, enumeration: Optional[DataContainer] = None) -> any:
    """
    Converts a string to the appropriate JSON field value
    :param value: The string to parse
    :param schema: The schema string to convert the value with
    :param enumeration: The enumeration to validate the data against (only needed for enumeration fields)
    :return: The parsed JSON field value
    :raises ValueError: If the provided schema is not a recognized schema value
    """
    if schema == 'i':
        return int(value)
    elif schema == 'u':
        return validate_unsigned_int_field(value)
    elif schema == 'v':
        return validate_positive_int_field(value)
    elif schema == 'x':
        return int(value, 16)
    elif schema == 'f':
        return float(value)
    elif schema == 'b':
        return bool(value)
    elif schema == 'n':
        return validate_name_field(value)
    elif schema == 's':
        return value
    elif schema == 'q':
        return value
    elif schema == 'm':
        return value
    elif schema == 'e':
        return check_enum_field(value, enumeration)
    elif schema == 'y':
        if re.match(r'-?\d+$', value):
            return int(value)

        return check_enum_field(value, enumeration)

    raise ValueError("Failed to parse field '{0}' with provided schema value '{1}'".format(value, schema))


def validate_name_field(value: str) -> str:
    """
    Validate a field that must represent name value
    :param value: The string to validate
    :return: The validate name string
    :raises ValueError: If the provided string does not conform to the schema defined
    """
    if not re.match(r'^(?!\d)\w+$', value):
        raise ValueError("Field '{0}' must contain only letters, digits, and underscores and can't "
                         "begin with a number.".format(value))

    return value


def validate_unsigned_int_field(value: str) -> int:
    """
    Validate that a string value is an unsigned integer
    :param value: The value to validate
    :return: The integer representation of the value
    :raises ValueError: If the provided value is negative
    """
    ret = int(value)
    if ret < 0:
        raise ValueError('Field {0} is not a positive integer or 0.'.format(value))

    return ret


def validate_positive_int_field(value: str) -> int:
    """
    Validate that a string value is a positive integer
    :param value: The value to validate
    :return: The integer representation of the value
    :raises ValueError: If the provided value is 0 or negative
    """
    ret = int(value)
    if ret <= 0:
        raise ValueError('Field {0} is not a positive integer.'.format(value))

    return ret


def check_enum_field(field: str, enumeration: Optional[DataContainer]) -> str:
    """
    Validate that a field is contained within an enumeration
    :param field: The string to validate against the enum
    :param enumeration: The set of possible values to consider
    :return: The validated value
    :raises ValueError: If the provided value is not in the enumeration
    """
    if enumeration is None:
        warn("Field '{0}' was specified as an enumeration, but no enumeration was provided! "
             "Validation will be skipped".format(field))
    elif field not in enumeration:
        raise ValueError("Undefined value {0} in {1}.".format(field, str(enumeration)))

    return field
