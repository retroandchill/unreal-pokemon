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
#include "GameFramework/PlayerController.h"
#include "RPGPlayerController.generated.h"

class UScreen;
class USelectableWidget;
class UInputAction;
class UInputMappingContext;
/**
 * Player controller responsible for RPG menu related actions and behaviors.
 */
UCLASS(Blueprintable)
class RPGMENUS_API ARPGPlayerController : public APlayerController {
	GENERATED_BODY()

public:
	/**
	 * Add a screen of the given class to the stack
	 * @tparam T The screen class to spawn
	 * @return The created screen.
	 */
	template <typename T, typename = std::enable_if_t<std::is_base_of_v<UScreen, T>>>
	T* AddScreenToStack(TSubclassOf<T> ScreenClass = T::StaticClass()) {
		auto Screen = CreateWidget<T>(this, ScreenClass);
        Screen->AddToViewport();
        ScreenStack.Add(Screen);
        return Screen;
	}

	/** Creates a widget */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta=( WorldContext="WorldContextObject", DisplayName="Add Screen to Stack", BlueprintInternalUseOnly="true" ), Category="Widget")
	static UScreen* AddScreenToStackHelper(UObject* WorldContextObject, TSubclassOf<UScreen> ScreenType);

private:
	/**
	 * The mapping context asset to use to determine navigation controls
	 */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	/**
	 * The input to use for moving the cursor around the menu
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TSoftObjectPtr<UInputAction> CursorInput;

	/**
	 * The input to use for moving the cursor around the menu
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TSoftObjectPtr<UInputAction> ConfirmInput;

	/**
	 * The input to use for canceling the
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TSoftObjectPtr<UInputAction> CancelInput;

	/**
	 * The internal stack of screens used to handle the input.
	 */
	UPROPERTY()
	TArray<TObjectPtr<UScreen>> ScreenStack;
};
