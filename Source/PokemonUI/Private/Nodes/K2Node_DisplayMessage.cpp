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
#include "Nodes/K2Node_DisplayMessage.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintFunctionNodeSpawner.h"
#include "BlueprintNodeSpawner.h"
#include "EdGraphSchema_K2.h"
#include "K2Node_AsyncAction.h"
#include "K2Node_CallFunction.h"
#include "K2Node_TemporaryVariable.h"
#include "KismetCompiler.h"
#include "RPGPlayerController.h"
#include "Nodes/DisplayMessage.h"
#include "Nodes/MessageNode.h"
#include "Screens/TextDisplayScreen.h"

class UK2Node_AsyncAction;

UK2Node_DisplayMessage::UK2Node_DisplayMessage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
}

void UK2Node_DisplayMessage::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const {
	auto FactoryFunc = UDisplayMessage::StaticClass()->FindFunctionByName("DisplayMessage");
	check(FactoryFunc != nullptr);
	SupplyMenuActions(ActionRegistrar, FactoryFunc);
}

void UK2Node_DisplayMessage::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) {
	static const FName OnConfirmPinName(TEXT("OnConfirm"));

	auto OnConfirmPin = FindPinChecked(OnConfirmPinName);
	ReconnectOutputPin(CompilerContext, OnConfirmPin);
	
	Super::ExpandNode(CompilerContext, SourceGraph);
}
