// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintGraphPanelPinFactory.h"
#include "EdGraphUtilities.h"

/**
 * Factory class to handling the visuals of pins for the Pokémon Data module
 */
class POKEMONDATADEVELOPER_API FPokemonDataGraphPinFactory : public FGraphPanelPinFactory {
public:
	FPokemonDataGraphPinFactory(const FPokemonDataGraphPinFactory&) = default;
	FPokemonDataGraphPinFactory(FPokemonDataGraphPinFactory&&) noexcept = default;
	~FPokemonDataGraphPinFactory() = default;

	FPokemonDataGraphPinFactory& operator=(const FPokemonDataGraphPinFactory&) = default;
	FPokemonDataGraphPinFactory& operator=(FPokemonDataGraphPinFactory&&) noexcept = default;

	TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* Pin) const override;
};
