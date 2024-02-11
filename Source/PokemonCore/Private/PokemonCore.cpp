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
#include "PokemonCore.h"

#include "Pokemon/Exp/Erratic.h"
#include "Pokemon/Exp/Fast.h"
#include "Pokemon/Exp/Fluctuating.h"
#include "Pokemon/Exp/Medium.h"
#include "Pokemon/Exp/Parabolic.h"
#include "Pokemon/Exp/Slow.h"

constexpr auto GLoctextNamespace = "FPokemonCoreModule";

void FPokemonCoreModule::StartupModule() {
	auto& GrowthRateRegistry = Exp::FGrowthRateRegistry::GetInstance();
	GrowthRateRegistry.RegisterClass<Exp::FMedium>("Medium");
	GrowthRateRegistry.RegisterClass<Exp::FErratic>("Erratic");
	GrowthRateRegistry.RegisterClass<Exp::FFluctuating>("Fluctuating");
	GrowthRateRegistry.RegisterClass<Exp::FParabolic>("Parabolic");
	GrowthRateRegistry.RegisterClass<Exp::FFast>("Fast");
	GrowthRateRegistry.RegisterClass<Exp::FSlow>("Slow");
}

void FPokemonCoreModule::ShutdownModule() {
    // No implementation needed
}
    
IMPLEMENT_MODULE(FPokemonCoreModule, PokemonCore)