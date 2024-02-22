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
#include "Screen.generated.h"

class USelectableWidget;
struct FInputActionInstance;
/**
 * Represents a basic screen used by the UI. They tend to be added in a stack format, and are displayed one on top
 * of the other.
 */
UCLASS(Blueprintable, Abstract)
class RPGMENUS_API UScreen : public UUserWidget {
	GENERATED_BODY()

public:
	/**
	 * Construct the default version of the screen
	 * @param ObjectInitializer The initializer used by Unreal Engine to build the object
	 */
	explicit UScreen(const FObjectInitializer& ObjectInitializer);
	
	TSharedRef<SWidget> RebuildWidget() override;

protected:
	/**
	 * Close the screen and return to the previous one
	 */
	UFUNCTION(BlueprintCallable, Category = Navigation)
	void CloseScreen();

private:
	/**
	 * Get list of selectable widgets in the window
	 */
	UPROPERTY()
	TArray<TObjectPtr<USelectableWidget>> SelectableWidgets;
};
