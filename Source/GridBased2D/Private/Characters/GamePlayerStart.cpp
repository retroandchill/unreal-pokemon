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
#include "Characters/GamePlayerStart.h"

#include "GridUtils.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AGamePlayerStart::AGamePlayerStart(const FObjectInitializer& ObjectInitializer) : APlayerStart(ObjectInitializer) {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	double Radius = GridBased2D::GGridSize / 2;;
	auto Capsule = GetCapsuleComponent();
	Capsule->SetCapsuleRadius(Radius);
	Capsule->SetCapsuleHalfHeight(Radius);
}

// Called when the game starts or when spawned
void AGamePlayerStart::BeginPlay() {
	Super::BeginPlay();
	
}

// Called every frame
void AGamePlayerStart::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

