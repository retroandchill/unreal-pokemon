// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Events/BattleMessage.h"

FBattleMessage::FBattleMessage(FText &&Message) : Message(MoveTemp(Message)) {
}