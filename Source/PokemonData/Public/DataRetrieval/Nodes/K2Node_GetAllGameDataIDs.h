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
#include "K2Node_GetGameData.h"
#include "K2Node_GetAllGameDataIDs.generated.h"

/**
 * Custom Blueprint node to get all the IDs from a data table
 */
UCLASS()
class POKEMONDATA_API UK2Node_GetAllGameDataIDs : public UK2Node {
	GENERATED_BODY()

public:
	/**
	 * Set up the node assigning the struct that this should be retrieving
	 * @param NodeStruct The struct type for this node
	 */
	void Initialize(const UScriptStruct* NodeStruct);

	void AllocateDefaultPins() override;
	FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	FText GetTooltipText() const override;
	bool IsNodePure() const override;
	FText GetMenuCategory() const override;
	FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;

	void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override; 

private:
	/**
	 * The struct type represented by this node
	 */
	UPROPERTY()
	TObjectPtr<const UScriptStruct> StructType;
};
