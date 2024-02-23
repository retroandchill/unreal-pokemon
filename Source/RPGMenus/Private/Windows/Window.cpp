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
#include "Windows/Window.h"

#include "Data/Windowskin.h"

UWindow::UWindow(const FObjectInitializer& ObjectInitializer) : UWidget(ObjectInitializer) {
	Brush.DrawAs = ESlateBrushDrawType::Box;
}

TSharedRef<SWidget> UWindow::RebuildWidget() {
	return SNew(SImage).Image(&Brush);
}

void UWindow::SynchronizeProperties() {
	Super::SynchronizeProperties();
	
	if (Windowskin != nullptr) {
		Brush.TintColor = FSlateColor(FColor(255, 255, 255));

		auto SourceTexture = Windowskin->GetSourceTexture();
		auto &Margins = Windowskin->GetMargins();
		double TextureWidth = SourceTexture->GetSizeX();
		double TextureHeight = SourceTexture->GetSizeY();
		
		Brush.SetResourceObject(SourceTexture);
		Brush.Margin = FMargin(Margins.Left / TextureWidth, Margins.Top / TextureHeight,
			Margins.Right / TextureWidth, Margins.Bottom / TextureHeight);
	} else {
		Brush.TintColor = FSlateColor(FColor(0, 0, 0, 0));
		Brush.SetResourceObject(nullptr);
	}
}
