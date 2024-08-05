// "Unreal Pokémon" created by Retro & Chill.


#include "Field/FieldItemEffect.h"

UWorld * UFieldItemEffect::GetWorld() const {
    return GetOuter()->GetWorld();
}

FDelegateHandle UFieldItemEffect::BindToEffectComplete(FOnItemEffectComplete::FDelegate &&Delegate) {
    return OnEffectComplete.Add(MoveTemp(Delegate));
}

void UFieldItemEffect::EffectComplete(bool bSuccess) const {
    OnEffectComplete.Broadcast(bSuccess);
}