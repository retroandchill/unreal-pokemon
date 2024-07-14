﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGMenuUtilities.generated.h"

class UScreen;
/**
 * 
 */
UCLASS()
class RPGMENUS_API URPGMenuUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    /**
     * Helper function used to create a screen on the stack from Blueprints
     * @param WorldContextObject The world context object needed to get the subsystem
     * @param ScreenType The screen class to spawn
     * @return The created screen.
     */
    UFUNCTION(BlueprintCallable, Category = Screens)
    static UScreen *PushScreenToStack(const UObject *WorldContextObject, TSubclassOf<UScreen> ScreenType);
    
    UFUNCTION(BlueprintCallable, Category = Screens)
    static UScreen* RemoveTopScreenFromOverlay(const UObject *WorldContextObject);

};
