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
#include "WindowFrame.generated.h"

class UImage;
/**
 * Widget class to represent the frame of the window
 */
UCLASS(Blueprintable, Abstract)
class RPGMENUS_API UWindowFrame : public UUserWidget {
	GENERATED_BODY()

public:
	/**
	 * Construct the default version of the window
	 * @param ObjectInitializer The initializer used by Unreal Engine to build the object
	 */
	explicit UWindowFrame(const FObjectInitializer& ObjectInitializer);

	TSharedRef<SWidget> RebuildWidget() override;

	/** 
	* Change the windowskin and redraw the windowskin components 
	* @param NewWindowskin The next windowskin texture to use
	*/
	UFUNCTION(BlueprintCallable, Category = Window)
	void ChangeWindowskin(UTexture2D *NewWindowskin);

private:
	/**
	 * The source texture used to make the window
	 */
	UPROPERTY(EditAnywhere, Category = Windowskin)
	TObjectPtr<UTexture2D> SourceTexture;

	/**
	 * The image used to construct the top left part of the window
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> WindowBackground;

	/**
	 * The image used to construct the top left part of the window
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UNamedSlot> ContentsPane;
};
