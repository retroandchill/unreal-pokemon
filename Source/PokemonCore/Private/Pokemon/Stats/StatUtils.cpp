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
#include "Pokemon/Stats/StatUtils.h"

#include "DataManager.h"
#include "Species/Nature.h"
#include "Species/Stat.h"

using namespace StatUtils;

POKEMONCORE_API TMap<FName, int32> StatUtils::RandomizeIVs() {
	auto &DataSubsystem = FDataManager::GetInstance();
	auto &StatTable = DataSubsystem.GetDataTable<FStat>();

	TMap<FName, int32> Ret;
	StatTable.ForEach([&Ret](const FStat &Stat) {
		if (Stat.Type == EPokemonStatType::Battle)
			return;

		Ret[Stat.ID] = FMath::RandRange(0, 31);
	});

	return Ret;
}

POKEMONCORE_API TMap<FName, int32> StatUtils::DefaultEVs() {
	auto &DataSubsystem = FDataManager::GetInstance();
	auto &StatTable = DataSubsystem.GetDataTable<FStat>();

	TMap<FName, int32> Ret;
	StatTable.ForEach([&Ret](const FStat &Stat) {
		if (Stat.Type == EPokemonStatType::Battle)
			return;

		Ret[Stat.ID] = 0;
	});

	return Ret;
}

POKEMONCORE_API FName StatUtils::RandomNature() {
	auto &DataSubsystem = FDataManager::GetInstance();
	auto &NatureTable = DataSubsystem.GetDataTable<FNature>();

	auto Rows = NatureTable.GetTableRowNames().Array();
	auto Index = FMath::RandRange(0, Rows.Num() - 1);
	return Rows[Index];
}

POKEMONCORE_API TUniquePtr<Exp::IGrowthRate> StatUtils::CreateGrowthRate(FName GrowthRate) {
	return Exp::FGrowthRateRegistry::GetInstance().Construct(GrowthRate);
}

POKEMONCORE_API int32 StatUtils::GetMaxLevel() {
	// TODO: Allow this to be configurable
	return 100;
}
