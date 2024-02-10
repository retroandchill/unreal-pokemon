//====================================================================================================================
// ** Unreal Pokémon created by Retro & Chill
//--------------------------------------------------------------------------------------------------------------------
// This project is intended as a means of learning more about how a game like Pokémon works by creating a framework
// from the ground up, and for non-commercial applications. While this code is original, Pokémon is the intellectual
// property of Game Freak and Nintendo, as such it is highly discouraged to use this kit to make a commercial product.
//--------------------------------------------------------------------------------------------------------------------
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//====================================================================================================================
#include "Pokemon/Stats/DefaultMainStatEntry.h"

FDefaultMainStatEntry::FDefaultMainStatEntry(FName Stat, int32 IV, int32 EV) : FDefaultStatEntry(Stat, IV, EV) {
}

void FDefaultMainStatEntry::RefreshValue(int32 Level, int32 Base, const FNature&) {
	SetStatValue((2 * Base + GetIV() + GetEV() / 4) * Level / 100 + Level + 10);
}

TUniquePtr<IStatEntry> FDefaultMainStatEntry::Clone() const {
	return MakeUnique<FDefaultMainStatEntry>(*this);
}
