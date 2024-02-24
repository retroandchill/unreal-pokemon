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
#include "DisplayText.generated.h"

class UMenuCommand;
class UTextBlock;

/**
 * Basic text command widget used for handling an option in a menu
 */
UCLASS(Blueprintable, Abstract)
class RPGMENUS_API UDisplayText : public UUserWidget {
	GENERATED_BODY()

public:
	/**
	 * Construct the default version of the window
	 * @param ObjectInitializer The initializer used by Unreal Engine to build the object
	 */
	explicit UDisplayText(const FObjectInitializer& ObjectInitializer);

	TSharedRef<SWidget> RebuildWidget() override;

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

	/**
	 * Get the font to be displayed to the player
	 * @return The font to set for the window
	 */
	UFUNCTION(BlueprintPure, Category = Text)
	const FSlateFontInfo& GetDisplayFont() const;

	/**
	 * Get the size of the current text contained within this widget
	 * @return The size of the current text
	 */
	FVector2D GetTextSize() const;

	/**
	 * Get the size of the given text when displayed to the player
	 * @param Text The text to display to the player
	 * @return The size of the given text
	 */
	FVector2D GetTextSize(const FString& Text) const;

	/**
	 * Get the total size of the text drawing area
	 * @return The total size of the area to draw the text in
	 */
	FVector2D GetTotalTextAreaSize() const;

	/**
	 * Get the padding around the display text for the player
	 * @return The padding around the text
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Text|Size")
	FMargin GetDisplayTextPadding() const;
	
protected:
	/**
	 * Called when the text is set by the game to make sure all Blueprint Visuals are good
	 */
	UFUNCTION(BlueprintNativeEvent, Category = Text)
	void OnTextSet(const FText &Text);

	/**
	 * Get the primary display text widget
	 * @return The displayed text widget to the player
	 */
	UFUNCTION(BlueprintPure, Category = "Widgets|Text")
	UTextBlock *GetDisplayTextWidget() const;
	
private:
	/**
	 * The displayed text widget to the player
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DisplayTextWidget;

	/**
	 * The font to set for the window
	 */
	UPROPERTY(EditAnywhere, Category = Display, meta=(UIMin = 1, ClampMin = 1))
	FSlateFontInfo DisplayFont;
};
