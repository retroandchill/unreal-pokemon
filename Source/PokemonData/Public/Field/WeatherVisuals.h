// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeatherVisuals.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UWeatherVisuals : public UInterface {
	GENERATED_BODY()
};

/**
 * Interface for handling the visuals of the weather effects on the field
 */
class POKEMONDATA_API IWeatherVisuals {
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Apply the effect of the weather on the screen
	 * @param Strength The strength of the weather used to adjust the tone by
	 */
	UFUNCTION()
	virtual void ApplyToneChange(float Strength) = 0;
};
