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
#include "UObject/Object.h"
#include "Windowskin.generated.h"

class UPaperSprite;
/**
 * Basic Windowskin that contains the necessary assets in order to construct a Window frame
 */
UCLASS(BlueprintType)
class RPGMENUS_API UWindowskin : public UObject {
	GENERATED_BODY()

public:

	/**
	 * Get one of the parts of the windowskin
	 * @param Index The index to look for the window part from
	 * @return The sprite asset used for the window
	 */
	UFUNCTION(BlueprintPure, Category = "Windowskin")
	UPaperSprite *GetWindowPart(int32 Index) const;
	
private:
	/**
	 * The 9 sub-components of the Windowskin image that make up the entire window.
	 *
	 * Starts from the top left and moves left-to-right up-to-down
	 */
	UPROPERTY(EditAnywhere, EditFixedOrder, Category = "Windowskin")
	TArray<TObjectPtr<UPaperSprite>> WindowskinParts = {
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr
	};
};
