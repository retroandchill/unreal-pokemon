// "Unreal Pokémon" created by Retro & Chill.
#include "GridBasedGameModeBase.h"

#include "Characters/GamePlayer.h"

AGridBasedGameModeBase::AGridBasedGameModeBase() {
	DefaultPawnClass = AGamePlayer::StaticClass();
}
