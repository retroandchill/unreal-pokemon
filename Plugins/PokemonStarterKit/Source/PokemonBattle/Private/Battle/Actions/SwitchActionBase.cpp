// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Actions/SwitchActionBase.h"
#include "Battle/Tags.h"

USwitchActionBase::USwitchActionBase() {
    auto &AbilityTrigger = AbilityTriggers.Emplace_GetRef();
    AbilityTrigger.TriggerTag = Pokemon::Battle::SwitchOut;
}