// "Unreal Pokémon" created by Retro & Chill.

#include "Field/FieldItemEffect.h"

#if WITH_EDITOR
bool UFieldItemEffect::ImplementsGetWorld() const {
    return true;
}
#endif

FDelegateHandle UFieldItemEffect::BindToEffectComplete(FOnItemEffectComplete::FDelegate &&Delegate) {
    return OnEffectComplete.Add(MoveTemp(Delegate));
}

void UFieldItemEffect::EffectComplete(bool bSuccess) const {
    OnEffectComplete.Broadcast(bSuccess);
}