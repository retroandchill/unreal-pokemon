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
#include "Primatives/DisplayText.h"
#include "ShadowedText.generated.h"

/**
 * Text widget that consists of three (3) additional text shadows that are displayed.
 */
UCLASS(Blueprintable, Abstract)
class POKEMONUI_API UShadowedText : public UDisplayText {
	GENERATED_BODY()

public:
	TSharedRef<SWidget> RebuildWidget() override;

	void OnTextSet_Implementation(const FText& Text) override;

private:
	/**
	 * Internal method used to set the text and font of a shadow
	 * @param Shadow A pointer to the shadow to set the information of
	 */
	void SetShadowTextAndFont(UTextBlock* Shadow);

	/**
	 * Set the text of the given shadow
	 * @param Shadow A pointer to the shadow
	 * @param Text The text to set
	 */
	void SetShadowText(UTextBlock* Shadow, const FText &Text);
	
	/**
	 * The first text shadow to display
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Shadow1;

	/**
	 * The second text shadow to display
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Shadow2;

	/**
	 * The third text shadow to display
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Shadow3;
};
