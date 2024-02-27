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
#include "Primatives/DisplayText.h"

#include "CanvasItem.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Fonts/FontMeasure.h"
#include "Utilities/WidgetUtilities.h"

UDisplayText::UDisplayText(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer) {
}

TSharedRef<SWidget> UDisplayText::RebuildWidget() {
	auto Ret = Super::RebuildWidget();

	if (DisplayTextWidget != nullptr) {
		DisplayTextWidget->SetFont(DisplayFont);

		if (SizeBox != nullptr) {
			auto TextPadding = GetDisplayTextPadding();
			SizeBox->SetHeightOverride(GetTextSize().Y + TextPadding.Top + TextPadding.Bottom);
		}
	}
	
	return Ret;
}

FText UDisplayText::GetText() const {
	check(DisplayTextWidget != nullptr);
	return DisplayTextWidget->GetText();
}

void UDisplayText::SetText(const FText& NewText) {
	if (DisplayTextWidget != nullptr) {
		DisplayTextWidget->SetText(NewText);
		
		if (SizeBox != nullptr) {
			auto TextPadding = GetDisplayTextPadding();
			SizeBox->SetHeightOverride(GetTextSize().Y + TextPadding.Top + TextPadding.Bottom);
		}
		
		OnTextSet(NewText);
	}
}

const FSlateFontInfo& UDisplayText::GetDisplayFont() const {
	return DisplayFont;
}

FVector2D UDisplayText::GetTextSize() const {
	check(DisplayTextWidget != nullptr);
	return GetTextSize(DisplayTextWidget->GetText().ToString());
}

FVector2D UDisplayText::GetTextSize(const FString& Text) const {
	check(DisplayTextWidget != nullptr);
	auto FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
	FVector2D Size = FontMeasure->Measure(Text, DisplayFont, UWidgetUtilities::GetWidgetDPIScale());
	return Size;
}

FVector2D UDisplayText::GetTotalTextAreaSize() const {
	return DisplayTextWidget->GetCachedGeometry().GetLocalSize();
}

void UDisplayText::OnTextSet_Implementation(const FText& Text) {
	// No definition needed here in this class
}

UTextBlock* UDisplayText::GetDisplayTextWidget() const {
	return DisplayTextWidget;
}

