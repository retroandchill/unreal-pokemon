// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Events/BattleMessagePayload.h"

const FRunningMessageSet & UBattleMessagePayload::GetRunningMessageSet() const {
    return Messages;
}