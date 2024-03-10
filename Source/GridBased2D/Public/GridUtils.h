// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Characters/FacingDirection.h"

namespace GridBased2D {
	/**
	 * The size of the grid according to the game
	 */
	constexpr double GRID_SIZE = 32.0f;

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
	void AdjustMovementPosition(EFacingDirection MovementDirection, Vector& Position) {
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

	/**
	 * Get the direction directly opposite the provided direction
	 * @param Direction The original direction
	 * @return The opposing direction
	 */
	GRIDBASED2D_API TOptional<EFacingDirection> GetOpposingDirection(EFacingDirection Direction);
}
