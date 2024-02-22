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
#include "RPGPlayerController.h"

#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"

#include "Screens/Screen.h"

ARPGPlayerController::ARPGPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = false;
}

void ARPGPlayerController::BeginPlay() {
	Super::BeginPlay();
	SetInputMode(FInputModeGameOnly());
}

UScreen* ARPGPlayerController::AddScreenToStackHelper(UObject* WorldContextObject, TSubclassOf<UScreen> ScreenType) {
	if (ScreenType == nullptr)
		return nullptr;
	
	if (auto ImpliedOwningPlayer = Cast<ARPGPlayerController>(WorldContextObject); ImpliedOwningPlayer != nullptr) {
		return ImpliedOwningPlayer->AddScreenToStack(ScreenType);
	}

	if (auto OwningWidget = Cast<UUserWidget>(WorldContextObject); OwningWidget != nullptr)
	{
		if (auto RPGPlayerController = Cast<ARPGPlayerController>(OwningWidget->GetOwningPlayer()); RPGPlayerController != nullptr) {
			return RPGPlayerController->AddScreenToStack(ScreenType);
		}
	}

	if (auto World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (auto RPGPlayerController = Cast<ARPGPlayerController>(World->GetFirstPlayerController()); RPGPlayerController != nullptr) {
			return RPGPlayerController->AddScreenToStack(ScreenType);
		}
	}

	return nullptr;
}
