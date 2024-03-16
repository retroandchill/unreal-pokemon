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
import unittest
from unittest.mock import MagicMock

from pokemon.data_loader import UnrealDataLoader


class MyTestCase(unittest.TestCase):
    def test_load_data(self):
        data = {'item1', 'item2', 'item3'}
        create_data = MagicMock(return_value=data)
        data_loader = UnrealDataLoader(create_data)
        for val in data:
            self.assertIn(val, data_loader)

        create_data.assert_called_once()

    def test_set_getter_later(self):
        data = {'item1', 'item2', 'item3'}
        create_data = MagicMock(return_value=data)
        data_loader = UnrealDataLoader()
        data_loader.set_getter(create_data)
        for val in data:
            self.assertIn(val, data_loader)

        create_data.assert_called_once()

    def test_invalid_lazy_loader(self):
        data_loader = UnrealDataLoader()
        with self.assertRaises(AttributeError):
            if 'item1' in data_loader:
                pass

    def test_double_set_getter(self):
        data = {'item1', 'item2', 'item3'}
        create_data = MagicMock(return_value=data)
        data_loader = UnrealDataLoader()
        data_loader.set_getter(create_data)

        new_getter = MagicMock(return_value=set())
        with self.assertRaises(AttributeError):
            data_loader.set_getter(new_getter)


if __name__ == '__main__':
    unittest.main()
