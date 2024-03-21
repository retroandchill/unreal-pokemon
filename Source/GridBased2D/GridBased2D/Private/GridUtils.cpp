// "Unreal Pokémon" created by Retro & Chill.
#include "GridUtils.h"

#include "GridBasedGameModeBase.h"
#include "Kismet/GameplayStatics.h"

TOptional<EFacingDirection> UGridUtils::VectorToFacingDirection(const FVector2D Vector) {
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

TOptional<EFacingDirection> UGridUtils::GetOpposingDirection(EFacingDirection Direction) {
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

AGridBasedGameModeBase* UGridUtils::GetGridBasedGameMode(const UObject* WorldContext) {
	auto GameMode = Cast<AGridBasedGameModeBase>(UGameplayStatics::GetGameMode(WorldContext));
	ASSERT(GameMode != nullptr)
	return GameMode;
}
