// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Effects/ApplyMessageGameplayEffectComponent.h"
#include "PokemonBattleModule.h"
#include "Utilities/PokemonCoroutineDispatcher.h"

UE5Coro::TCoroutine<> UApplyMessageGameplayEffectComponent::DisplayApplyMessage(UE5Coro::TLatentContext<const UObject> Context, const FText &TargetName) const {
    if (MessageFormat.IsEmptyOrWhitespace()) {
        UE_LOG(LogBattle, Error, TEXT("You must provide a message."));
        co_return;
    }
    
    auto &Dispatcher = IPokemonCoroutineDispatcher::Get(Context.Target);
    co_await Dispatcher.DisplayMessage(FText::FormatNamed(MessageFormat, "Target", TargetName));
}