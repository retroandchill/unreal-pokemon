// "Unreal Pokémon" created by Retro & Chill.

#include "Pokemon/TrainerMemo/DefaultObtainedBlock.h"
#include "Managers/PokemonSubsystem.h"
#include "Pokemon/PokemonDTO.h"

TScriptInterface<IObtainedBlock> UDefaultObtainedBlock::Initialize(const FPokemonDTO &DTO) {
    ObtainMethod = DTO.ObtainMethod;
    LevelMet = DTO.LevelMet.Get(DTO.Level);

    // clang-format off
    ObtainText = DTO.MetLocation |
                 Retro::Optionals::OrElseGet([this] {
                     return UPokemonSubsystem::Exists(this)
                                ? UPokemonSubsystem::GetInstance(this).GetCurrentLocation()
                                : FText();
                 });
    // clang-format on

    TimeReceived = DTO.TimeReceived.Get(FDateTime::Now());
    TimeHatched = DTO.TimeHatched;
    HatchedMap = DTO.HatchedMap;
    return this;
}

EObtainMethod UDefaultObtainedBlock::GetObtainMethod() const {
    return ObtainMethod;
}

int32 UDefaultObtainedBlock::GetLevelMet() const {
    return LevelMet;
}

TOptional<const FDateTime &> UDefaultObtainedBlock::GetTimeReceived() const {
    return &TimeReceived;
}

TOptional<FText> UDefaultObtainedBlock::GetObtainText() {
    return ObtainText;
}

TOptional<const FDateTime &> UDefaultObtainedBlock::GetTimeHatched() const {
    return TimeHatched.GetPtrOrNull();
}

void UDefaultObtainedBlock::SetTimeHatched(const FDateTime &DateTime) {
    TimeHatched = DateTime;
}

TOptional<FText> UDefaultObtainedBlock::GetHatchedMap() {
    return HatchedMap;
}

void UDefaultObtainedBlock::SetHatchedMap(const FText &MapName) {
    HatchedMap = MapName;
}