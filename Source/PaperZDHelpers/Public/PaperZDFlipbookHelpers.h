// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Characters/FacingDirection.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PaperZDFlipbookHelpers.generated.h"

class UPaperZDAnimSequence;
class UPaperFlipbook;

/**
 * 
 */
UCLASS()
class PAPERZDHELPERS_API UPaperZDFlipbookHelpers : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "PaperZD|Rendering")
    static UPaperFlipbook* GetPreviewFlipbook(UPaperZDAnimSequence* Sequence, EFacingDirection Direction);

};
