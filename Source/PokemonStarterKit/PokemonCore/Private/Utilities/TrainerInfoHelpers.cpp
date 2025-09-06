// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/TrainerInfoHelpers.h"
#include "Pokemon/TrainerMemo/ObtainedBlock.h"
#include "RetroLib/Optionals/BlueprintBridge.h"

ESetStatus UTrainerInfoHelpers::GetTimeReceived(const TScriptInterface<IObtainedBlock> &ObtainedBlock,
                                                FDateTime &TimeReceived)
{
    return Retro::Optionals::GetBlueprintOptionalValue(ObtainedBlock->GetTimeReceived(), TimeReceived);
}

ESetStatus UTrainerInfoHelpers::GetObtainText(const TScriptInterface<IObtainedBlock> &ObtainedBlock, FText &ObtainText)
{
    return Retro::Optionals::GetBlueprintOptionalValue(ObtainedBlock->GetObtainText(), ObtainText);
}

ESetStatus UTrainerInfoHelpers::GetTimeHatched(const TScriptInterface<IObtainedBlock> &ObtainedBlock,
                                               FDateTime &TimeHatched)
{
    return Retro::Optionals::GetBlueprintOptionalValue(ObtainedBlock->GetTimeHatched(), TimeHatched);
}

ESetStatus UTrainerInfoHelpers::GetHatchedMap(const TScriptInterface<IObtainedBlock> &ObtainedBlock, FText &HatchedMap)
{
    return Retro::Optionals::GetBlueprintOptionalValue(ObtainedBlock->GetHatchedMap(), HatchedMap);
}