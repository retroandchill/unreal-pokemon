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
#include "DataSubsystem.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Exp/GrowthRateData.h"
#include "Species/GenderRatio.h"
#include "Species/EggGroup.h"
#include "Species/BodyShape.h"
#include "Species/BodyColor.h"
#include "Species/Habitat.h"
#include "Species/EvolutionData.h"
#include "Species/Stat.h"
#include "Species/Nature.h"
#include "Battle/Status.h"
#include "Field/Weather.h"
#include "Field/EncounterType.h"
#include "Battle/Environment.h"
#include "Battle/BattleWeather.h"
#include "Battle/BattleTerrain.h"
#include "Moves/Target.h"

#include "Bag/Item.h"
#include "DataRetrieval/DataRegistry.h"

UDataSubsystem::UDataSubsystem() {
	auto &AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetData;
	AssetRegistryModule.Get().GetAssetsByClass(FTopLevelAssetPath(UDataTable::StaticClass()->GetPathName()), AssetData);
	for (auto &Iter : AssetData) {
		auto Table = Cast<UDataTable>(Iter.GetAsset());
		if (Table == nullptr)
			continue;

		auto RowStruct = Table->GetRowStruct();
		auto &DataRegistry = FDataRegistry::GetInstance();
		if (!DataRegistry.IsTypeRegistered(RowStruct))
			continue;
		
		DataTables.Add(RowStruct->GetFName(), DataRegistry.CreateDataTableProxy(RowStruct, Table));
	}
}

const IGameData& UDataSubsystem::GetDataTable(TObjectPtr<const UScriptStruct> StructType) const {
	check(StructType != nullptr);
	auto StructName = StructType->GetFName();
	check(DataTables.Contains(StructName));
	return *DataTables[StructName];
}
