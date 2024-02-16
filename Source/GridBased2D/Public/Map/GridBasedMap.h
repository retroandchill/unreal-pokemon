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
#include "GameFramework/Actor.h"
#include "PaperTileMapComponent.h"
#include "GridBasedMap.generated.h"

UCLASS(Blueprintable, ClassGroup=(Map))
class GRIDBASED2D_API AGridBasedMap : public AActor {
	GENERATED_BODY()

public:
	/**
	 * Sets default values for this actor's properties
	 */
	AGridBasedMap();

	void PostInitProperties() override;
	void PostReinitProperties() override;
	void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent);
	void PostLoad() override;
	void PostEditMove(bool bFinished) override;

protected:
	void BeginPlay() override;

public:
	void Tick(float DeltaTime) override;

private:
	/**
	 * Set up the location of the map based on the configured properties
	 */
	void SetUpMapLocation();

	/**
	 * The TileMap object that this map holds.
	 */
	UPROPERTY(EditAnywhere, Category = "Display")
	TObjectPtr<UPaperTileMap> TileMap;
	
	/**
	 * The tilemap to use for this asset
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Map, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPaperTileMapComponent> TileMapComponent;

	/**
	 * The layer of the tilemap that is at the same level as the player
	 */
	UPROPERTY(EditAnywhere, Category = "Display", meta = (UIMin = 0, ClampMin = 0))
	int32 PlayerLevelLayer;
};
