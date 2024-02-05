from typing import Callable, Optional, Union


class UnrealDataLoader:
    def __init__(self, getter: Optional[Callable[[], set[str]]] = None):
        self.__getter = getter
        self.__data: Optional[set[str]] = None

    def __contains__(self, item: str) -> bool:
        if self.__data is None:
            if self.__getter is not None:
                self.__data = self.__getter()
            else:
                raise AttributeError("Invalid getter for lazy loading!")

        return item in self.__data

    def set_getter(self, getter: Callable[[], set[str]]):
        if self.__getter is None:
            self.__getter = getter
        else:
            raise AttributeError("Getter already assigned!")


DataContainer = Union[set[str], UnrealDataLoader]
