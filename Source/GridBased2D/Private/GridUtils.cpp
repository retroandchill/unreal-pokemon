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
#include "GridUtils.h"

TOptional<EFacingDirection> GridBased2D::VectorToFacingDirection(const FVector2D Vector) {
	using enum EFacingDirection;
	if (Vector.Y > 0) {
		return Down;
	}

	if (Vector.X < 0 ) {
		return Left;
	}

	if (Vector.X > 0) {
		return Right;
	}

	if (Vector.Y < 0) {
		return Up;
	}

	return TOptional<EFacingDirection>();
}

TOptional<EFacingDirection> GridBased2D::GetOpposingDirection(EFacingDirection Direction) {
	switch (Direction) {
		using enum EFacingDirection;
	case Down:
		return Up;
	case Left:
		return Right;
	case Right:
		return Left;
	case Up:
		return Down;
	}

	return TOptional<EFacingDirection>();
}
