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
#include "Nodes/K2Node_DisplayMessageWithChoices.h"

#include "Nodes/DisplayMessageWithChoices.h"

UK2Node_DisplayMessageWithChoices::UK2Node_DisplayMessageWithChoices(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
}

void UK2Node_DisplayMessageWithChoices::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const {
	auto FactoryFunc = UDisplayMessageWithChoices::StaticClass()->FindFunctionByName("DisplayMessageWithChoices");
	check(FactoryFunc != nullptr);
	SupplyMenuActions(ActionRegistrar, FactoryFunc);
}

void UK2Node_DisplayMessageWithChoices::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) {
	Super::ExpandNode(CompilerContext, SourceGraph);
}
