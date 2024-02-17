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
#include "UObject/Object.h"
#include "MenuCommand.generated.h"

/**
 * Delegate for when the user presses confirm
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FProcessCommand);

/**
 * Object that contains a command in the menu
 */
UCLASS(BlueprintType, EditInlineNew)
class RPGMENUS_API UMenuCommand : public UObject {
	GENERATED_BODY()

public:
	/**
	 * Get the display text for the command
	 * @return The text for the option that is displayed directly to the player
	 */
	UFUNCTION(BlueprintPure, Category = Menus)
	FText GetDisplayText() const;

private:
	/**
	 * The text for the option that is displayed directly to the player
	 */
	UPROPERTY(EditAnywhere, Category = Menus)
	FText DisplayText;

public:
	/**
	 * Delegate for when this menu option is selected
	 */
	UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = Events)
	FProcessCommand OnSelected;
	
};
