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
#include "Text/ShadowedText.h"

#include "Components/TextBlock.h"

TSharedRef<SWidget> UShadowedText::RebuildWidget() {
	auto Ret = Super::RebuildWidget();
	SetShadowTextAndFont(Shadow1);
	SetShadowTextAndFont(Shadow2);
	SetShadowTextAndFont(Shadow3);
	return Ret;
}

void UShadowedText::OnTextSet_Implementation(const FText& Text) {
	SetShadowText(Shadow1, Text);
	SetShadowText(Shadow2, Text);
	SetShadowText(Shadow3, Text);
}

UTextBlock* UShadowedText::GetShadow1() const {
	return Shadow1;
}

UTextBlock* UShadowedText::GetShadow2() const {
	return Shadow2;
}

UTextBlock* UShadowedText::GetShadow3() const {
	return Shadow3;
}

void UShadowedText::SetShadowTextAndFont(UTextBlock* Shadow) {
	if (Shadow != nullptr) {
		Shadow->SetText(GetText());
		Shadow->SetFont(GetDisplayFont());
	}
}

void UShadowedText::SetShadowText(UTextBlock* Shadow, const FText& Text) {
	check(Shadow != nullptr);
	Shadow->SetText(Text);
}
