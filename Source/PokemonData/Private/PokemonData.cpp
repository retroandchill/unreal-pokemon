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
#include "PokemonData.h"

#include "Bag/Item.h"
#include "Battle/BattleTerrain.h"
#include "Battle/BattleWeather.h"
#include "Battle/Environment.h"
#include "Battle/Status.h"
#include "DataRetrieval/DataRegistry.h"
#include "Exp/GrowthRateData.h"
#include "Field/EncounterType.h"
#include "Field/Weather.h"
#include "Moves/Target.h"
#include "Species/BodyColor.h"
#include "Species/BodyShape.h"
#include "Species/EggGroup.h"
#include "Species/EvolutionData.h"
#include "Species/GenderRatio.h"
#include "Species/Habitat.h"
#include "Species/Nature.h"
#include "Species/Stat.h"

constexpr auto GLoctextNamespace = "FPokemonDataModule";

void FPokemonDataModule::StartupModule() {
	auto& DataRegistry = FDataRegistry::GetInstance();
	DataRegistry.RegisterStruct<FGrowthRateData>();
	DataRegistry.RegisterStruct<FGenderRatio>();
	DataRegistry.RegisterStruct<FEggGroup>();
	DataRegistry.RegisterStruct<FBodyShape>();
	DataRegistry.RegisterStruct<FBodyColor>();
	DataRegistry.RegisterStruct<FHabitat>();
	DataRegistry.RegisterStruct<FEvolutionData>();
	DataRegistry.RegisterStruct<FStat>();
	DataRegistry.RegisterStruct<FNature>();
	DataRegistry.RegisterStruct<FStatus>();
	DataRegistry.RegisterStruct<FWeather>();
	DataRegistry.RegisterStruct<FEncounterType>();
	DataRegistry.RegisterStruct<FEnvironment>();
	DataRegistry.RegisterStruct<FBattleWeather>();
	DataRegistry.RegisterStruct<FBattleTerrain>();
	DataRegistry.RegisterStruct<FTarget>();

	DataRegistry.RegisterStruct<FItem>();
}

void FPokemonDataModule::ShutdownModule() {
	// No custom shutdown needed
}

IMPLEMENT_MODULE(FPokemonDataModule, PokemonData)
