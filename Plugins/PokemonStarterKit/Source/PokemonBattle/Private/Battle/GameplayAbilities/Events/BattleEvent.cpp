// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/GameplayAbilities/Events/BattleEvent.h"


IMPLEMENT_POLYMORPHIC_STRUCT(FBattleEventContext)
IMPLEMENT_POLYMORPHIC_STRUCT(FBattleEventValues)

TArray<FProperty*> Pokemon::Battle::Events::GetAllViableProperties(const UScriptStruct* StructType) {
    TArray<FProperty*> Ret;
    for (TObjectIterator<UScriptStruct> It; It; ++It) {
        if (!It->IsChildOf(StructType)) {
            continue;
        }

        for (TFieldIterator<FProperty> PropertyIt(*It, EFieldIteratorFlags::ExcludeSuper); PropertyIt; ++PropertyIt)
        {
            auto Property = *PropertyIt;

            // Allow properties to be filtered globally (never show up)
            if (Property->HasMetaData(TEXT("HideInDetailsView"))) {
                continue;
            }
				
            Ret.Add(Property);
        }
    }

    return Ret;
}