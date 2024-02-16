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
#include "Characters/Charset.h"

UPaperFlipbook* UCharset::GetDownSprite() const {
	return DownSprite.Flipbook;
}

UPaperFlipbook* UCharset::GetLeftSprite() const {
	return LeftSprite.Flipbook;
}

UPaperFlipbook* UCharset::GetRightSprite() const {
	return RightSprite.Flipbook;
}

UPaperFlipbook* UCharset::GetUpSprite() const {
	return UpSprite.Flipbook;
}

UPaperFlipbook* UCharset::GetSprite(EFacingDirection Direction) const {
	auto SpriteData = GetDirectionalSprite(Direction);
	if (SpriteData == nullptr)
		return nullptr;

	return SpriteData->Flipbook;
}

bool UCharset::CanStopOnFrame(EFacingDirection Direction, int32 CurrentFrame) const {
	auto SpriteData = GetDirectionalSprite(Direction);
	if (SpriteData == nullptr)
		return false;

	return SpriteData->ValidStopFrames.Contains(CurrentFrame);
}

const FDirectionalSprite* UCharset::GetDirectionalSprite(EFacingDirection Direction) const {
	switch (Direction) {
		using enum EFacingDirection;
	case Down:
		return &DownSprite;
	case Left:
		return &LeftSprite;
	case Right:
		return &RightSprite;
	case Up:
		return &UpSprite;
	}

	return nullptr;
}
