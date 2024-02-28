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
#include "Nodes/DisplayMessageWithChoices.h"

#include "RPGPlayerController.h"
#include "Screens/TextDisplayScreen.h"

UDisplayMessageWithChoices* UDisplayMessageWithChoices::DisplayMessageWithChoices(const UObject* WorldContextObject,
                                                                       TSubclassOf<UTextDisplayScreen> ScreenClass, FText Message, const TArray<FText>& Choices) {
	auto Node = NewObject<UDisplayMessageWithChoices>();
	Node->WorldContextObject = WorldContextObject;
	Node->ScreenClass = ScreenClass;
	Node->Message = Message;
	Node->Choices = Choices;
	return Node;
}

void UDisplayMessageWithChoices::Activate() {
	auto Controller = Cast<ARPGPlayerController>(WorldContextObject->GetWorld()->GetFirstPlayerController());
	if (Controller == nullptr)
		return;

	auto Screen = Controller->ConditionallyAddScreenToStack(ScreenClass);
	Screen->DisplayChoices(Message, Choices);
	Screen->ProcessChoice.AddDynamic(this, &UDisplayMessageWithChoices::ExecuteOnChoiceSelected);
}

void UDisplayMessageWithChoices::ExecuteOnChoiceSelected(int32 ChoiceIndex, FName ChoiceID) {
	OnChoiceSelected.Broadcast(ChoiceIndex, ChoiceID);

	
	auto Controller = Cast<ARPGPlayerController>(WorldContextObject->GetWorld()->GetFirstPlayerController());
	if (Controller == nullptr)
		return;

	auto Screen = Controller->GetTopOfStack<UTextDisplayScreen>();
	if (Screen == nullptr)
		return;

	Screen->ProcessChoice.RemoveDynamic(this, &UDisplayMessageWithChoices::ExecuteOnChoiceSelected);
}
