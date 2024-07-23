// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "PaperZDEditorHelpers.generated.h"

class UPaperFlipbook;
class UPaperZDAnimationSource;

/**
 *
 */
UCLASS()
class UNREALPOKEMONEDITOR_API UPaperZDEditorHelpers : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Set the target animation source for the given factory.
     * <p><b>NOTE</b>: This function is incredibly unsafe, call at your own risk.</p>
     * @param Factory The source factory object
     * @param AnimationSource The target animation source
     * @return Did this operation succeed?
     */
    UFUNCTION(BlueprintCallable, Category = "PaperZD|Utilties")
    static bool SetTargetAnimSource(UFactory *Factory, UPaperZDAnimationSource *AnimationSource);
};
