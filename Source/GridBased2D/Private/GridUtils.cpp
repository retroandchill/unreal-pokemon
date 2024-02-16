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

GRIDBASED2D_API double GridBased2D::LinearInterpolation(double StartValue, double EndValue, double Duration, double Delta) {
	if (Duration <= 0)
		return EndValue;

	if (Delta <= 0)
		return StartValue;

	if (Delta >= Duration)
		return EndValue;

	return StartValue + (EndValue - StartValue) * Delta / Duration;
}

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
