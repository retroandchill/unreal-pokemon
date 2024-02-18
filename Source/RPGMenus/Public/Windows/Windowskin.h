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

/**
 * Represents a valid Windowskin the the editor
 */
UCLASS(BlueprintType, HideCategories=(Object))
class RPGMENUS_API UWindowskin : public UObject {
	GENERATED_BODY()

public:
	/**
	 * Get the source texture for the window
	 * @return The source texture used to make the window
	 */
	UTexture2D* GetSourceTexture() const;

	/**
	 * Get the margins for the window
	 * @return The size of the margins for the window in pixel offset from the edges
	 */
	const FBox2D &GetMargins() const;

private:
	/**
	 * The source texture used to make the window
	 */
	UPROPERTY(EditAnywhere, Category = Windowskin)
	TObjectPtr<UTexture2D> SourceTexture;

	/**
	 * The size of the margins for the window in pixel offset from the edges
	 */
	UPROPERTY(EditAnywhere, Category = Windowskin)
	FBox2D Margins;
};
