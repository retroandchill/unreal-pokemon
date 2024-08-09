// "Unreal Pokémon" created by Retro & Chill.

#include "Field/FieldItemEffect.h"

bool UFieldItemEffect::ImplementsGetWorld() const {
    return true;
}

FDelegateHandle UFieldItemEffect::BindToEffectComplete(FOnItemEffectComplete::FDelegate &&Delegate) {
    return OnEffectComplete.Add(MoveTemp(Delegate));
}

void UFieldItemEffect::EffectComplete(bool bSuccess) const {
    OnEffectComplete.Broadcast(bSuccess);
}