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
#include "Exp/GrowthRate.h"
#include "Exp/GrowthRateData.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GetGrowthRateTest, "Tests.Exp.GetGrowthRateTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool GetGrowthRateTest::RunTest(const FString& Parameters) {
	auto GameInstance = NewObject<UGameInstance>();
	GameInstance->Init();
	auto DataSubsystem = GameInstance->GetSubsystem<UDataSubsystem>();

	bool Passes = true;
	auto& GrowthRateProxy = DataSubsystem->GetDataTable<FGrowthRateData>();
	auto IdList = GrowthRateProxy.GetTableRowNames();
	Passes &= TestNotEqual("No Growth Rates Found!", IdList.Num(), 0);
	for (auto ID : IdList) {
		Passes &= TestTrue("Failed to find ID in list!", GrowthRateProxy.IsRowNameValid(ID));

		auto GrowthRate = GrowthRateProxy.GetData(ID);
		if (!TestNotNull("Growth rate should not be null!", GrowthRate)) {
			Passes = false;
			continue;
		}

		Passes &= TestEqual("IDs do not match!", GrowthRate->ID, ID);

		auto ImplementationClass = NewObject<UObject>(DataSubsystem, GrowthRate->ImplementationClass);
		auto AsInterface = Cast<IGrowthRate>(ImplementationClass);
		Passes &= TestNotNull("Implementation class does not Implement Growth Rate!", AsInterface);
	}

	return Passes;
}
