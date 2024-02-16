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
#pragma once

#include "CoreMinimal.h"
#include "Characters/FacingDirection.h"

namespace GridBased2D {
	/**
	 * The size of the grid according to the game
	 */
	constexpr double GGridSize = 32.0;

	/**
	 * Linear interpolation between two values, given the duration of the change and the time passed since the start of
	 * the change (delta)
	 * @param StartValue The starting value for the change
	 * @param EndValue Where the value should be at the end of the duration
	 * @param Duration The duration of the change
	 * @param Delta The change in time
	 * @return The interpolated value
	 */
	GRIDBASED2D_API double LinearInterpolation(double StartValue, double EndValue, double Duration, double Delta);

	/**
	 * Convert a vector into a facing direction
	 * @param Vector The input vector
	 * @return The interpreted direction
	 */
	GRIDBASED2D_API TOptional<EFacingDirection> VectorToFacingDirection(const FVector2D Vector);

	/**
	 * Adjust the movement position based upon the given direction
	 * @tparam Vector The vector type to adjust the position of
	 * @param MovementDirection The direction to move in
	 * @param Position The position struct to alter
	 */
	template <typename Vector>
	void AdjustMovementPosition(EFacingDirection MovementDirection, Vector &Position) {
		switch (MovementDirection) {
			using enum EFacingDirection;
		case Down:
			Position.Y += 1;
			break;
		case Left:
			Position.X -= 1;
			break;
		case Right:
			Position.X += 1;
			break;
		case Up:
			Position.Y -= 1;
			break;
		}
	}
}
