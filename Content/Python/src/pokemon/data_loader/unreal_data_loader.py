# ====================================================================================================================
# ** Unreal Pokémon created by Retro & Chill
# --------------------------------------------------------------------------------------------------------------------
# This project is intended as a means of learning more about how a game like Pokémon works by creating a framework
# from the ground up, and for non-commercial applications. While this code is original, Pokémon is the intellectual
# property of Game Freak and Nintendo, as such it is highly discouraged to use this kit to make a commercial product.
# --------------------------------------------------------------------------------------------------------------------
# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
# THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
# ====================================================================================================================
from typing import Callable, Optional, Union


class UnrealDataLoader:
    """Lazy loader for Unreal data"""
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

    def set_getter(self, getter: Callable[[], set[str]]) -> None:
        """
        Setter for getter for lazy loading when __contains__ is called
        :param getter: The callable getter for the field
        :raises AttributeError: If the getter has already been assigned
        """
        if self.__getter is None:
            self.__getter = getter
        else:
            raise AttributeError("Getter already assigned!")


DataContainer = Union[set[str], UnrealDataLoader]
