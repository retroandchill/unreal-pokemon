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
#include "Windows/SelectableWindow.h"

USelectableWindow::USelectableWindow(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer) {
}

int32 USelectableWindow::GetItemCount_Implementation() const {
	return 0;
}

int32 USelectableWindow::GetRowCount() const {
	int32 ColumnCount = GetColumnCount();
	return (GetItemCount() + ColumnCount - 1) / ColumnCount; 
}

int32 USelectableWindow::GetColumnCount_Implementation() const {
	return 1;
}

int32 USelectableWindow::GetIndex() const {
	return Index;
}

void USelectableWindow::SetIndex(int32 NewIndex) {
	Index = NewIndex;
	OnSelectionChange(Index);
}

void USelectableWindow::Deselect() {
	Index = -1;
	OnSelectionChange(Index);
}

bool USelectableWindow::IsActive() const {
	return bActive;
}

void USelectableWindow::SetActive(bool bNewActiveState) {
	bActive = bNewActiveState;
}

void USelectableWindow::OnSelectionChange_Implementation(int32 NewIndex) {
	// No implementation, but we cannot have an abstract method in an Unreal class
}
