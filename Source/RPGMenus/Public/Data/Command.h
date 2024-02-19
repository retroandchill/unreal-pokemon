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
#include "Command.generated.h"

/**
 * The struct that holds the command information
 */
UCLASS(Blueprintable, EditInlineNew)
class RPGMENUS_API UCommand : public UObject {
	GENERATED_BODY()

public:
	/**
	 * Get the internal ID of the command
	 * @return The unique ID given to the command
	 */
	UFUNCTION(BlueprintPure, Category = Commands)
	FName GetID() const;

	/**
	 * Get the displayed text for the command
	 * @return The text displayed to the user
	 */
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = Commands)
	FText GetText() const;

	/**
	 * Get if this command should actually be added to the window
	 * @return Will this command be displayed?
	 */
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = Commands)
	bool IsEnabled() const;

	/**
	 * Get the color and opacity of this specific command
	 * @return The color and opacity of the command
	 */
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = Display)
	FSlateColor GetColorAndOpacity() const;

	/**
	 * Get the font information for the command
	 * @return The font to render the text with
	 */
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = Display)
	FSlateFontInfo GetFont() const;
	

protected:
	/**
	 * Get the original text without any additional formatting brought about by subclasses
	 * @return The text displayed to the user for the command in question
	 */
	UFUNCTION(BlueprintPure, Category = Commands)
	FText GetOriginalText() const;

	/**
	 * Get original color and opacity without any additional formatting brought about by subclasses
	 * @return The color of the text
	 */
	UFUNCTION(BlueprintPure, Category = Display)
	const FSlateColor& GetOriginalColorAndOpacity() const;

	/**
	 * Get original font without any additional formatting brought about by subclasses
	 * @return The font to render the text with
	 */
	UFUNCTION(BlueprintPure, Category = Display)
	const FSlateFontInfo& GetOriginalFont() const;

private:
	/**
	 * The unique ID given to the command
	 */
	UPROPERTY(EditAnywhere, Category = Commands)
	FName ID;

	/**
	 * The text displayed to the user for the command in question
	*/
	UPROPERTY(EditAnywhere, Category = Commands)
	FText Text;

	/**
	 * The color of the text
	 */
	UPROPERTY(EditAnywhere, Category = Display)
	FSlateColor ColorAndOpacity;

	/**
	 * The font to render the text with
	 */
	UPROPERTY(EditAnywhere, Category = Display)
	FSlateFontInfo Font; 
};
