// "Unreal Pokémon" created by Retro & Chill.


#include "Pokemon/TrainerMemo/DefaultObtainedBlock.h"
#include "DataTypes/OptionalUtilities.h"
#include "Pokemon/PokemonDTO.h"

TScriptInterface<IObtainedBlock> UDefaultObtainedBlock::Initialize(const FPokemonDTO &DTO) {
    ObtainMethod = DTO.ObtainMethod;
    LevelMet = DTO.Level;
    
    ObtainText = OptionalUtilities::OrElseGet(DTO.MetLocation, []() {
        // TODO: Get level name if level met is empty
        return FText::FromStringView(TEXT(""));
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

const FDateTime * UDefaultObtainedBlock::GetTimeReceived() const {
    return &TimeReceived;
}

TOptional<FText> UDefaultObtainedBlock::GetObtainText() {
    return ObtainText;
}

const FDateTime * UDefaultObtainedBlock::GetTimeHatched() const {
    return TimeHatched.GetPtrOrNull();
}

TOptional<FText> UDefaultObtainedBlock::GetHatchedMap() {
    return HatchedMap;
}