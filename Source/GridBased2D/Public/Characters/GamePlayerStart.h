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
#include "GameFramework/PlayerStart.h"
#include "GamePlayerStart.generated.h"

/**
 * Player start class with a capsule that matches the size of the grid.
 */
UCLASS(Blueprintable)
class GRIDBASED2D_API AGamePlayerStart : public APlayerStart {
	GENERATED_BODY()

public:
	/**
	 * Sets default values for this actor's properties
	 * @param ObjectInitializer The initialization parameters of the object
	 */
	explicit AGamePlayerStart(const FObjectInitializer& ObjectInitializer);

protected:
	void BeginPlay() override;

public:
	void Tick(float DeltaTime) override;
};
