// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"

#include "PokemonSpriteImporterBase.generated.h"

/**
 *
 */
UCLASS()
class UNREALPOKEMONEDITOR_API UPokemonSpriteImporterBase : public UEditorUtilityWidget
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = InputData, meta = (AllowPrivateAccess, ContentDir))
    FDirectoryPath BasePackage;
};
