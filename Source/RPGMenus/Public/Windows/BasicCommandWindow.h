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
#include "Menus/SelectableMenu.h"
#include "BasicCommandWindow.generated.h"

class UTextCommand;
/**
 * Struct for a simple menu command
 */
USTRUCT(BlueprintType)
struct FMenuCommand {
	GENERATED_BODY()

	/**
	 * The identifier for the command in the menu
	 */
	UPROPERTY(EditAnywhere, Category = Menus)
	FName ID;

	/**
	 * The text for the option that is displayed directly to the player
	 */
	UPROPERTY(EditAnywhere, Category = Menus)
	FText DisplayText;
};

/**
 * Basic command window, with a set of commands that can be dispatched
 */
UCLASS(Blueprintable, Abstract)
class RPGMENUS_API UBasicCommandWindow : public USelectableMenu {
	GENERATED_BODY()

public:
	int32 GetItemCount_Implementation() const override;

private:
	UPROPERTY(EditAnywhere, Category = Commands)
	TArray<TSoftObjectPtr<UTextCommand>> Commands;
};
