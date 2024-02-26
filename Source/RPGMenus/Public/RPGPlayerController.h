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
#include "Screens/Screen.h"
#include "RPGPlayerController.generated.h"

struct FInputActionInstance;
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
	explicit ARPGPlayerController(const FObjectInitializer &ObjectInitializer);
	
protected:
	void BeginPlay() override;
	
public:
	/**
	 * Add a screen of the given class to the stack
	 * @tparam T The screen class to spawn
	 * @return The created screen.
	 */
	template <typename T>
	requires std::is_base_of_v<UScreen, T>
	T* AddScreenToStack(TSubclassOf<T> ScreenClass = T::StaticClass()) {
		auto Screen = CreateWidget<T>(this, ScreenClass);
        Screen->AddToViewport();
        ScreenStack.Add(Screen);
		
		if (ScreenStack.Num() == 1) {
			SetInputMode(FInputModeUIOnly());
			SetShowMouseCursor(false);
		}
		Screen->SetKeyboardFocus();
		bShowMouseCursor = false;
		bEnableClickEvents = false;
		bEnableMouseOverEvents = false;
		
        return Screen;
	}

	/**
	 * Helper function used to create a screen on the stack from Blueprints
	 * @param WorldContextObject The world context object needed to get the controller
	 * @param ScreenType The screen class to spawn
	 * @return The created screen.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta=( WorldContext="WorldContextObject", DisplayName="Add Screen to Stack", BlueprintInternalUseOnly="true" ), Category="Widget")
	static UScreen* AddScreenToStackHelper(UObject* WorldContextObject, TSubclassOf<UScreen> ScreenType);


	/**
	 * Remove the current screen from the stack
	 * @return The screen to remove from the stack
	 */
	UScreen *RemoveScreenFromStack();

	/**
	 * Remove the current screen from the stack
	 * @param WorldContextObject The world context object needed to get the controller
	 * @return The screen to remove from the stack
	 */
	UFUNCTION(BlueprintCallable, Category="Widget", meta = (WorldContext = "WorldContextObject"))
	static UScreen* RemoveScreenFromStack(UObject* WorldContextObject);

private:
	/**
	 * The internal stack of screens used to handle the input.
	 */
	UPROPERTY()
	TArray<TObjectPtr<UScreen>> ScreenStack;
};
