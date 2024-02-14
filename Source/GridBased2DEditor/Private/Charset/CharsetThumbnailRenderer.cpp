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
#include "Charset/CharsetThumbnailRenderer.h"

#include "Characters/Assets/Charset.h"
#include "PaperFlipbook.h"
#include "ThumbnailRendering/ThumbnailManager.h"

bool UCharsetThumbnailRenderer::AllowsRealtimeThumbnails(UObject* Object) const {
	return true;
}

bool UCharsetThumbnailRenderer::CanVisualizeAsset(UObject* Object) {
	auto Charset = Cast<UCharset>(Object);
	if (Charset == nullptr)
		return false;

	return Charset->GetDownSprite() != nullptr;
}

void UCharsetThumbnailRenderer::Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height,
	FRenderTarget* RenderTarget, FCanvas* Canvas, bool bAdditionalViewFamily) {
	auto Charset = Cast<UCharset>(Object);
	if (Charset == nullptr)
		return;

	auto DownSprite = Charset->GetDownSprite();
	if (DownSprite == nullptr)
		return;

	auto ThumbnailInfo = UThumbnailManager::Get().GetRenderingInfo(DownSprite);
	if (ThumbnailInfo == nullptr || ThumbnailInfo->Renderer == nullptr)
		return;
	
	ThumbnailInfo->Renderer->Draw(DownSprite, X, Y, Width, Height, RenderTarget, Canvas, bAdditionalViewFamily);
}
