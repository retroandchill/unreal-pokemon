// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/TemporarySeed.h"

FTemporarySeed::FTemporarySeed(int32 Seed) {
    FMath::RandInit(Seed);
}

FTemporarySeed::~FTemporarySeed() {
    FMath::RandInit(static_cast<int32>(std::time(nullptr)));
}