// "Unreal Pokémon" created by Retro & Chill.


#include "Helpers/PaperZDEditorHelpers.h"
#include "UnrealPokemonEditor.h"

bool UPaperZDEditorHelpers::SetTargetAnimSource(UFactory *Factory, UPaperZDAnimationSource *AnimationSource) {
    static FName TargetAnimSource = "TargetAnimSource";
    auto Property = Factory->GetClass()->FindPropertyByName(TargetAnimSource);
    if (Property == nullptr) {
        UE_LOG(LogUnrealPokemonEditor, Error, TEXT("Provided factory does not have the 'TargetAnimSource' property"))
        return false;
    }

    auto PropertyContainer = Property->ContainerPtrToValuePtr<void>(Factory);
    TPropertyTypeFundamentals<TObjectPtr<UPaperZDAnimationSource>>::SetPropertyValue(PropertyContainer, AnimationSource);
    return true;
}