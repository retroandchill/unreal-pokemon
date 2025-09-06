// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"

#include "CharacterImporterBase.generated.h"

class UPaperZDAnimationSource;

/**
 * Base declaration of the editor import utility widget for characters to allow access to the C++ only Paper ZD
 * Animation Source Class.
 */
UCLASS()
class UNREALPOKEMONEDITOR_API UCharacterImporterBase : public UEditorUtilityWidget
{
    GENERATED_BODY()

  private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Output", meta = (AllowPrivateAccess))
    TObjectPtr<UPaperZDAnimationSource> AnimationSource;
};
