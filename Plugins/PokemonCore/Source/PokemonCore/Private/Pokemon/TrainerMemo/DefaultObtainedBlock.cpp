// "Unreal Pokémon" created by Retro & Chill.

#include "Pokemon/TrainerMemo/DefaultObtainedBlock.h"
#include "DataTypes/OptionalUtilities.h"
#include "Managers/PokemonSubsystem.h"
#include "Pokemon/PokemonDTO.h"

TScriptInterface<IObtainedBlock> UDefaultObtainedBlock::Initialize(const FPokemonDTO &DTO) {
    ObtainMethod = DTO.ObtainMethod;
    LevelMet = DTO.Level;
    
    ObtainText = OptionalUtilities::OrElseGet(DTO.MetLocation, [] {
        return UPokemonSubsystem::Exists() ? UPokemonSubsystem::GetInstance().GetCurrentLocation() : FText();
    });

    TimeReceived = FDateTime::Now();
    return this;
}

EObtainMethod UDefaultObtainedBlock::GetObtainMethod() const {
    return ObtainMethod;
}

int32 UDefaultObtainedBlock::GetLevelMet() const {
    return LevelMet;
}

const FDateTime *UDefaultObtainedBlock::GetTimeReceived() const {
    return &TimeReceived;
}

TOptional<FText> UDefaultObtainedBlock::GetObtainText() {
    return ObtainText;
}

const FDateTime *UDefaultObtainedBlock::GetTimeHatched() const {
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