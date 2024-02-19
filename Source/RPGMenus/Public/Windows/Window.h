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
#include "Components/Widget.h"
#include "Window.generated.h"

class SConstraintCanvas;
class UWindowskin;
/**
 * Basic Window Widget class
 */
UCLASS(BlueprintType, Category = Windows)
class RPGMENUS_API UWindow : public UWidget {
	GENERATED_BODY()

public:
	explicit UWindow(const FObjectInitializer &ObjectInitializer);
	TSharedRef<SWidget> RebuildWidget() override;
	void SynchronizeProperties() override;

private:
	/**
	 * The windowskin asset used to create the window
	 */
	UPROPERTY(EditAnywhere, Category = Windowskin)
	TObjectPtr<UWindowskin> Windowskin;
	
	/**
	 * The source texture used to make the window
	 */
	FSlateBrush Brush;
};
