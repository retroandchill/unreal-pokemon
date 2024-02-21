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
#include "CursorDirection.h"
#include "UObject/Object.h"
#include "SelectionInputs.generated.h"

/**
 * The input data used for selection in the menus.
 */
UCLASS(Blueprintable, EditInlineNew)
class RPGMENUS_API USelectionInputs : public UDataAsset {
	GENERATED_BODY()

public:
	TOptional<ECursorDirection> ParseInputs(const FKey &Key) const;
	
private:
	/**
	 * The Up Input
	 */
	UPROPERTY(EditAnywhere, Category = "Input|Navigation")
	TSet<FKey> UpInputs;

	/**
	 * The Down Input
	 */
	UPROPERTY(EditAnywhere, Category = "Input|Navigation")
	TSet<FKey> DownInputs;

	/**
	 * The Left Input
	 */
	UPROPERTY(EditAnywhere, Category = "Input|Navigation")
	TSet<FKey> LeftInputs;

	/**
	 * The Right Input
	 */
	UPROPERTY(EditAnywhere, Category = "Input|Navigation")
	TSet<FKey> RightInputs;

	/**
	 * The Confirm Input
	 */
	UPROPERTY(EditAnywhere, Category = "Input|Options")
	TSet<FKey> ConfirmInputs;

	/**
	 * The Cancel Input
	 */
	UPROPERTY(EditAnywhere, Category = "Input|Options")
	TSet<FKey> CancelInputs;
	
};
