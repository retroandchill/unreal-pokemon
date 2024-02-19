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
#include "Windowskin/WindowskinThumbnailRenderer.h"

#include "ThumbnailRendering/ThumbnailManager.h"
#include "Data/Windowskin.h"

bool UWindowskinThumbnailRenderer::AllowsRealtimeThumbnails(UObject* Object) const {
	return true;
}

bool UWindowskinThumbnailRenderer::CanVisualizeAsset(UObject* Object) {
	auto Windowskin = Cast<UWindowskin>(Object);
	if (Windowskin == nullptr)
		return false;

	return Windowskin->GetSourceTexture() != nullptr;
}

void UWindowskinThumbnailRenderer::Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height,
	FRenderTarget* RenderTarget, FCanvas* Canvas, bool bAdditionalViewFamily) {
		auto Windowskin = Cast<UWindowskin>(Object);
		if (Windowskin == nullptr)
			return;

		auto SourceTexture = Windowskin->GetSourceTexture();
		if (SourceTexture == nullptr)
			return;

		auto ThumbnailInfo = UThumbnailManager::Get().GetRenderingInfo(SourceTexture);
		if (ThumbnailInfo == nullptr || ThumbnailInfo->Renderer == nullptr)
			return;
	
		ThumbnailInfo->Renderer->Draw(SourceTexture, X, Y, Width, Height, RenderTarget, Canvas, bAdditionalViewFamily);
}
