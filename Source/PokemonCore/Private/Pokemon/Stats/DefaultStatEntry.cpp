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
#include "Pokemon/Stats/DefaultStatEntry.h"

#include "DataManager.h"


FDefaultStatEntry::FDefaultStatEntry(FName Stat, int32 IV, int32 EV) : StatID(Stat), IV(IV), EV(EV) {
}

int32 FDefaultStatEntry::GetStatValue() const {
	return Value;
}

const FStat& FDefaultStatEntry::GetStat() const {
	auto Stat = FDataManager::GetInstance().GetDataTable<FStat>().GetData(StatID);
	check(Stat != nullptr);

	return *Stat;
}

FName FDefaultStatEntry::GetStatID() const {
	return StatID;
}

int32 FDefaultStatEntry::GetIV() const {
	return IV;
}

int32 FDefaultStatEntry::GetEV() const {
	return EV;
}

void FDefaultStatEntry::SetStatValue(int32 NewValue) {
	Value = NewValue;
}
