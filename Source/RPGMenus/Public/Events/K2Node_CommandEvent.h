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
#include "EnhancedInputComponent.h"
#include "K2Node_Event.h"
#include "K2Node_CommandEvent.generated.h"

class UMenuCommand;
/**
 * Node called when using invoking a command to a delegate in an array
 */
UCLASS()
class RPGMENUS_API UK2Node_CommandEvent : public UK2Node_Event {
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UMenuCommand> MenuCommand;

	UPROPERTY()
	ETriggerEvent TriggerEvent;

	//~ Begin UK2Node Interface
	UClass* GetDynamicBindingClass() const override;
	void RegisterDynamicBinding(UDynamicBlueprintBinding* BindingObject) const override;
	void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	//~ End UK2Node Interface
};
