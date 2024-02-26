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
#include "Screens/Screen.h"
#include "TextDisplayScreen.generated.h"

class UMessageWindow;

/**
 * Handle advancing to the next message
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNextMessage);

/**
 * Screen for displaying text to the player
 */
UCLASS(Blueprintable, Abstract)
class POKEMONUI_API UTextDisplayScreen : public UScreen {
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	
	/**
	 * Set the text to display to the player from this menu
	 * @param TextToDisplay The text to display to the player
	 */
	UFUNCTION(BlueprintCallable, Category = "Messages|Display")
	void SetText(FText TextToDisplay);
	
	/**
	 * Clear all currently displaying text
	 */
	UFUNCTION(BlueprintCallable, Category = "Messages|Display")
	void ClearDisplayText();

	/**
	 * Assignable delegate for advancing to the next message
	 */
	UPROPERTY(BlueprintAssignable, Category = "Messages|Flow Control")
	FNextMessage NextMessage;
	
private:
	/**
	 * Function used to advance to the next message
	 */
	UFUNCTION()
	void AdvanceToNextMessage();
	
	/**
	 * The command window that is displayed to the player
	 */
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UMessageWindow> MessageWindow;
};
