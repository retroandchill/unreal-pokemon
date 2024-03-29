// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Asserts.h"
#include "EngineUtils.h"
#include "Characters/FacingDirection.h"
#include "GridUtils.generated.h"

class AGridBasedGameModeBase;
/**
 * Utility library for functions regarding the Grid system
 */
UCLASS()
class GRIDBASED2D_API UGridUtils : public UBlueprintFunctionLibrary {
	GENERATED_BODY()
public:
	/**
	 * The size of the grid according to the game
	 */
	static constexpr double GRID_SIZE = 32.0f;

	/**
	 * Convert a vector into a facing direction
	 * @param Vector The input vector
	 * @return The interpreted direction
	 */
	 static TOptional<EFacingDirection> VectorToFacingDirection(const FVector2D Vector);

	/**
	 * Adjust the movement position based upon the given direction
	 * @tparam Vector The vector type to adjust the position of
	 * @param MovementDirection The direction to move in
	 * @param Position The position struct to alter
	 */
	template <typename Vector>
	static void AdjustMovementPosition(EFacingDirection MovementDirection, Vector& Position) {
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
	static TOptional<EFacingDirection> GetOpposingDirection(EFacingDirection Direction);

	/**
	 * Get a list of all actors of the given type
	 * @tparam T The type of actor to look for
	 * @param WorldContext The object used to obtain the world context 
	 * @return The found actors
	 */
	template <typename T>
	requires std::is_base_of_v<AActor, T>
	static TArray<T*> FindAllActors(const UObject* WorldContext) {
		TArray<T*> Ret;
		auto World = WorldContext->GetWorld();
		ASSERT(World != nullptr)

		for (TActorIterator<T> It(World); It; ++It) {
			Ret.Emplace(*It);
		}

		return Ret;
	}

	/**
	 * Returns the current Game Mode as a GridBasedGameModeBase
	 * @param WorldContext The context object used to retrieve the game mode
	 * @return The found game mode
	 */
	UFUNCTION(BlueprintPure, Category = "Game", meta=(WorldContext="WorldContext"))
	static AGridBasedGameModeBase* GetGridBasedGameMode(const UObject* WorldContext);
};
