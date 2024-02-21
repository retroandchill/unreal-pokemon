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
#include "Blueprint/UserWidget.h"
#include "TextCommand.generated.h"

class UMenuCommand;
class UTextBlock;

/**
* Delegate for when the user presses confirm
*/
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FProcessCommand);

/**
 * Basic text command widget used for handling an option in a menu
 */
UCLASS(Blueprintable, Abstract)
class RPGMENUS_API UTextCommand : public UUserWidget {
	GENERATED_BODY()

public:
	/**
	 * Construct the default version of the window
	 * @param ObjectInitializer The initializer used by Unreal Engine to build the object
	 */
	explicit UTextCommand(const FObjectInitializer& ObjectInitializer);

	/**
	 * Get the display text as shown to the player
	 * @return The display text
	 */
	UFUNCTION(BlueprintPure, Category = Text)
	FText GetText() const;

	/**
	 * Set the display text for this widget
	 * @param NewText The new display text for the widget
	 */
	void SetText(const FText& NewText);

private:
	/**
	 * The displayed text widget to the player
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DisplayTextWidget;
};
