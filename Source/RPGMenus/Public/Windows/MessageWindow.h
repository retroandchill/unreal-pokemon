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
#include "MessageWindow.generated.h"

class UScrollBox;
class UWindow;
class USizeBox;
class UDisplayText;
class USelectionInputs;
/**
 * Window for display text to the player
 */
UCLASS()
class RPGMENUS_API UMessageWindow : public UUserWidget {
	GENERATED_BODY()

public:
	TSharedRef<SWidget> RebuildWidget() override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/**
	 * Set the text to display to the player
	 * @param Text The text to display to the player
	 */
	UFUNCTION(BlueprintCallable, Category = "Messages|Display")
	void SetDisplayText(FText Text);

	/**
	 * Clear all currently displaying text
	 */
	UFUNCTION(BlueprintCallable, Category = "Messages|Display")
	void ClearDisplayText();

private:
	/**
	 * Queue up the new text if the geometry information is available
	 */
	void QueueUpNewText();

	/**
	 * Queue up a new line of text to display to the player
	 * @param Line The current line of text to queue
	 * @param TotalTextAreaWidth The total width of the rendering area
	 */
	void QueueLine(const FString& Line, double TotalTextAreaWidth);

	/**
	 * Queue the given text to the display queue
	 * @param Text The text to add to the queue
	 */
	void QueueText(FStringView Text);

	/**
	 * Split up the line by word and add each word individually wrapping when necessary
	 * @param Line The current line of text to queue
	 * @param TotalTextAreaWidth The total width of the rendering area
	 */
	void QueueIndividualWords(const FString& Line, double TotalTextAreaWidth);

	/**
	 * Queue a new line to be added
	 */
	void AddNewLine();
	
	/**
	 * The actual area where the window is drawn
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWindow> Window;
	
	/**
	 * The widget that contains the text displayed to the player
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDisplayText> DisplayTextWidget;

	/**
	 * The scroll box used to keep the text visible on screen
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox;
	
	/**
	 * The size box used to control the desired height
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox;
	
	/**
	 * Do the selection options wrap when input would exceed the end
	 */
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<USelectionInputs> InputMappings;

	/**
	 * The full text to queue up for display
	 */
	TOptional<FText> FullText;

	/**
	 * The current word to be displayed to the player
	 */
	TQueue<TCHAR> WordToDisplay;

	/**
	 * The speed at which the text scrolls by (0 = Instant)
	 */
	UPROPERTY(EditAnywhere, Category = Display, meta = (UIMin = 0, UIMax = 0))
	float TextSpeed = 0.025f;

	/**
	 * The current timer for the text letter display
	 */
	float TextTimer = 0.0f;
	
	/**
	 * The desired number of lines to display to the player
	 */
	UPROPERTY(EditAnywhere, Category = Display, meta=(UIMin = 1, ClampMin = 1))
	int32 LinesToShow = 2;
	
};
