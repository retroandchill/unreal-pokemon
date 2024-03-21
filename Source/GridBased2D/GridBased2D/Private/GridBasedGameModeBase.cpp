// "Unreal Pokémon" created by Retro & Chill.
#include "GridBasedGameModeBase.h"

#include "Characters/GamePlayer.h"

AGridBasedGameModeBase::AGridBasedGameModeBase() {
	DefaultPawnClass = AGamePlayer::StaticClass();
}

void AGridBasedGameModeBase::FadeIn(const FScreenTransitionCallback& Callback) {
	OnScreenTransitionFinished.Clear();
	OnScreenTransitionFinished.Add(Callback);
	ScreenFadeIn();
}

void AGridBasedGameModeBase::FadeIn() {
	OnScreenTransitionFinished.Clear();
	ScreenFadeIn();
}

void AGridBasedGameModeBase::FadeOut(const FScreenTransitionCallback& Callback) {
	OnScreenTransitionFinished.Clear();
	OnScreenTransitionFinished.Add(Callback);
	ScreenFadeOut();
}

void AGridBasedGameModeBase::FadeOut() {
	OnScreenTransitionFinished.Clear();
	ScreenFadeOut();
}
