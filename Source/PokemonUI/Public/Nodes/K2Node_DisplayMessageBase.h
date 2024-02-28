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
#pragma once

#include "CoreMinimal.h"
#include "K2Node_BaseAsyncTask.h"
#include "MessageNode.h"
#include "K2Node_DisplayMessageBase.generated.h"

class UTextDisplayScreen;
/**
 * Basic template of any async node that requires the display of a message
 */
UCLASS(Abstract)
class POKEMONUI_API UK2Node_DisplayMessageBase : public UK2Node_BaseAsyncTask, public IMessageNode {
	GENERATED_BODY()

public:
	/**
	 * Default construct the class using the given initializer
	 * @param ObjectInitializer The Unreal provided initializer
	 */
	explicit UK2Node_DisplayMessageBase(const FObjectInitializer& ObjectInitializer);
	
	/**
	 * Set up the node assigning the struct that this should be retrieving
	 * @param NodeClass The screen type for this node
	 * @param NodeCounter The internal counter for how many nodes there are
	 */
	void Initialize(TSubclassOf<UTextDisplayScreen> NodeClass, TSharedRef<uint32> NodeCounter);

	void AllocateDefaultPins() override;
	FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

protected:
	/**
	 * Add the supplied menu actions for each Display Text Screen subclass
	 * @param ActionRegistrar Used to register nodes
	 * @param FactoryFunc The function used to actually produce the nodes
	 */
	void SupplyMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar, UFunction* FactoryFunc) const;

	/**
	 * Check if the supplied pin is connected to another message related node, and if not, add a node to close the window
	 * @param CompilerContext The context to compile in
	 * @param OutputPin The pin to alter the connections to
	 */
	void ReconnectOutputPin(FKismetCompilerContext& CompilerContext, UEdGraphPin* OutputPin);

	
private:
	/**
	 * The class referenced by this node
	 */
	UPROPERTY()
	TSubclassOf<UTextDisplayScreen> ScreenType;

	/**
	 * How many total screens are there
	 */
	TSharedRef<uint32> TotalScreens = MakeShared<uint32>(0);
};
