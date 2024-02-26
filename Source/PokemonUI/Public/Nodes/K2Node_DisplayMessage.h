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
#include "K2Node.h"
#include "K2Node_BaseAsyncTask.h"
#include "K2Node_DisplayMessage.generated.h"

class UTextDisplayScreen;
/**
 * Blueprint node for handling the display of a message to the player
 */
UCLASS()
class POKEMONUI_API UK2Node_DisplayMessage : public UK2Node_BaseAsyncTask {
	GENERATED_BODY()

public:
	/**
	 * Default construct the class using the given initializer
	 * @param ObjectInitializer The Unreal provided initializer
	 */
	explicit UK2Node_DisplayMessage(const FObjectInitializer& ObjectInitializer);
	
	/**
	 * Set up the node assigning the struct that this should be retrieving
	 * @param NodeClass The screen type for this node
	 * @param NodeCounter The internal counter for how many nodes there are
	 */
	void Initialize(TSubclassOf<UTextDisplayScreen> NodeClass, TSharedRef<uint32> NodeCounter);

	void AllocateDefaultPins() override;
	FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;

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
