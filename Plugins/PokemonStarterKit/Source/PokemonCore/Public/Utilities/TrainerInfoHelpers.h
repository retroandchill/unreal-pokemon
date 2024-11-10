// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Ranges/Optional/BlueprintBridge.h"

#include "TrainerInfoHelpers.generated.h"

class IObtainedBlock;
/**
 * 
 */
UCLASS()
class POKEMONCORE_API UTrainerInfoHelpers : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    /**
     * Retrieves the time when a Pokémon was received.
     *
     * @param ObtainedBlock The interface containing the obtained block data.
     * @param TimeReceived The output parameter where the received time will be stored.
     * @return An ESetStatus indicating whether the time was successfully retrieved or not.
     */
    UFUNCTION(BlueprintCallable, Category = "TrainerInfoHelpers",
        meta = (DefaultToSelf = ObtainedBlock, AutoCreateRefTerm = ObtainedBlock, ExpandEnumAsExecs = ReturnValue))
    static ESetStatus GetTimeReceived(const TScriptInterface<IObtainedBlock>& ObtainedBlock, FDateTime& TimeReceived);

    /**
     * Retrieves the text describing how a Pokémon was obtained.
     *
     * @param ObtainedBlock The interface containing the obtained block data.
     * @param ObtainText The output parameter where the obtain text will be stored.
     * @return An ESetStatus indicating whether the obtain text was successfully retrieved or not.
     */
    UFUNCTION(BlueprintCallable, Category = "TrainerInfoHelpers",
        meta = (DefaultToSelf = ObtainedBlock, AutoCreateRefTerm = ObtainedBlock, ExpandEnumAsExecs = ReturnValue))
    static ESetStatus GetObtainText(const TScriptInterface<IObtainedBlock>& ObtainedBlock, FText& ObtainText);

    /**
     * Retrieves the time when a Pokémon hatched.
     *
     * @param ObtainedBlock The interface containing the obtained block data.
     * @param TimeHatched The output parameter where the hatched time will be stored.
     * @return An ESetStatus indicating whether the time was successfully retrieved or not.
     */
    UFUNCTION(BlueprintCallable, Category = "TrainerInfoHelpers",
        meta = (DefaultToSelf = ObtainedBlock, AutoCreateRefTerm = ObtainedBlock, ExpandEnumAsExecs = ReturnValue))
    static ESetStatus GetTimeHatched(const TScriptInterface<IObtainedBlock>& ObtainedBlock, FDateTime& TimeHatched);

    /**
     * Retrieves the map where a Pokémon hatched.
     *
     * @param ObtainedBlock The interface containing the obtained block data.
     * @param HatchedMap The output parameter where the hatched map will be stored.
     * @return An ESetStatus indicating whether the map was successfully retrieved or not.
     */
    UFUNCTION(BlueprintCallable, Category = "TrainerInfoHelpers",
        meta = (DefaultToSelf = ObtainedBlock, AutoCreateRefTerm = ObtainedBlock, ExpandEnumAsExecs = ReturnValue))
    static ESetStatus GetHatchedMap(const TScriptInterface<IObtainedBlock>& ObtainedBlock, FText& HatchedMap);

};
