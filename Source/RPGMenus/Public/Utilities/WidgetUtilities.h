// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WidgetUtilities.generated.h"

/**
 * Utility class for Widget related operations
 */
UCLASS()
class RPGMENUS_API UWidgetUtilities : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	
	/**
	 * Get the DPI scale of the UMG widgets
	 * @return The current DPI scale
	 */
	UFUNCTION(BlueprintPure, DisplayName = "Get Widget DPI Scale", Category = "UMG|Scaling")
	static float GetWidgetDPIScale();
};
