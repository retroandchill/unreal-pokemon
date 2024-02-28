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
#include "Nodes/K2Node_DisplayMessageBase.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Screens/TextDisplayScreen.h"

UK2Node_DisplayMessageBase::UK2Node_DisplayMessageBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	ProxyActivateFunctionName = GET_FUNCTION_NAME_CHECKED(UBlueprintAsyncActionBase, Activate);
}

void UK2Node_DisplayMessageBase::Initialize(TSubclassOf<UTextDisplayScreen> NodeClass, TSharedRef<uint32> NodeCounter) {
	ScreenType = MoveTemp(NodeClass);
	TotalScreens = MoveTemp(NodeCounter);
}

void UK2Node_DisplayMessageBase::AllocateDefaultPins() {
	Super::AllocateDefaultPins();
	static const FName ScreenType_ParamName(TEXT("ScreenClass"));
	auto ScreenClassPin = FindPinChecked(ScreenType_ParamName);
	ScreenClassPin->DefaultObject = ScreenType;
	ScreenClassPin->bHidden = true;
}

FText UK2Node_DisplayMessageBase::GetNodeTitle(ENodeTitleType::Type TitleType) const {
	if (ScreenType != nullptr && *TotalScreens > 1) {
		const auto& ScreenName = ScreenType->GetName();
		return FText::FormatNamed(
			NSLOCTEXT("K2Node", "DisplayMessage_NodeTitleFormat", "{OriginalName} ({ClassName})"), TEXT("OriginalName"),
				Super::GetNodeTitle(TitleType), TEXT("ClassName"), FText::FromString(ScreenName));
	}

	return Super::GetNodeTitle(TitleType);
}

void UK2Node_DisplayMessageBase::SupplyMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar, UFunction* FactoryFunc) const {
	auto CustomizeCallback = [](UEdGraphNode* Node, [[maybe_unused]] bool bIsTemplateNode,
								TSubclassOf<UTextDisplayScreen> Subclass, TSharedRef<uint32> NodeCounter, UFunction *FactoryFunc) {
		
		auto TypedNode = CastChecked<UK2Node_DisplayMessageBase>(Node);
		auto ReturnProp = CastFieldChecked<FObjectProperty>(FactoryFunc->GetReturnProperty());
						
		TypedNode->ProxyFactoryFunctionName = FactoryFunc->GetFName();
		TypedNode->ProxyFactoryClass        = FactoryFunc->GetOuterUClass();
		TypedNode->ProxyClass               = ReturnProp->PropertyClass;
		TypedNode->Initialize(Subclass, MoveTemp(NodeCounter));
	};

	auto ScreenCounter = MakeShared<uint32>(0);
	for (TObjectIterator<UClass> It; It; ++It) {
		if (!It->IsChildOf(UTextDisplayScreen::StaticClass()) || It->HasAnyClassFlags(CLASS_Abstract))
			continue;
		
		if (!UEdGraphSchema_K2::IsAllowableBlueprintVariableType(*It, true))
			continue;

		auto Spawner = UBlueprintNodeSpawner::Create(GetClass());
		check(Spawner != nullptr);

		(*ScreenCounter)++;
		Spawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(CustomizeCallback, TSubclassOf<UTextDisplayScreen>(*It), ScreenCounter, FactoryFunc);
		ActionRegistrar.AddBlueprintAction(GetClass(), Spawner);
	}
}
