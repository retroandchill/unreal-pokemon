from typing import Callable, Optional, Union


class UnrealDataLoader:
    def __init__(self, getter: Callable[[], set[str]]):
        self.__getter = getter
        self.__data: Optional[set[str]] = None

    def __contains__(self, item: str) -> bool:
        if self.__data is None:
            self.__data = self.__getter()

        return item in self.__data


DataContainer = Union[set[str], UnrealDataLoader]
