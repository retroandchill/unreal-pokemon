// "Unreal Pokémon" created by Retro & Chill.
#include "GridUtils.h"

TOptional<EFacingDirection> GridBased2D::VectorToFacingDirection(const FVector2D Vector) {
	using enum EFacingDirection;
	if (Vector.Y > 0) {
		return Down;
	}

	if (Vector.X < 0) {
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
