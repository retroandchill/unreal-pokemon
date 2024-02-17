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
#include "K2Node_CommandEvent.h"
#include "Engine/DynamicBlueprintBinding.h"
#include "CommandBlueprintBinding.generated.h"

class UTextCommand;
class UBasicCommandWindow;
class UMenuCommand;
/**
 * Binding for the command option in the menu
 */
USTRUCT()
struct RPGMENUS_API FBlueprintCommandBinding
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<const UMenuCommand> MenuCommand = nullptr;

	UPROPERTY()
	ETriggerEvent TriggerEvent = ETriggerEvent::None;

	UPROPERTY()
	FName FunctionNameToBind = NAME_None;
	
};

/**
 * Dynamic binding for the Command window
 */
UCLASS()
class RPGMENUS_API UCommandBlueprintBinding : public UDynamicBlueprintBinding {
	GENERATED_BODY()

public:
	/**
	 * The list of valid command bindings
	 */
	UPROPERTY()
	TArray<FBlueprintCommandBinding> CommandBindings;

	/**
	 * Bind the event to the dispatcher on the commands
	 * @param CommandWidget The command widget to bind the command to
	 * @param ObjectToBindTo The binding object for the callback
	 */
	void BindToCommandWindow(UTextCommand* CommandWidget, UObject* ObjectToBindTo) const;
};
