// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Moves/MoveLookup.h"
#include "Battle/Moves/PokemonBattleMove.h"
#include "Battle/Moves/BattleMove.h"
#include "Settings/BaseSettings.h"


TSubclassOf<UGameplayAbility> Pokemon::Battle::Moves::LookupMoveEffectClass(FName FunctionCode) {
    if (FunctionCode.IsNone()) {
        return UPokemonBattleMove::StaticClass();
    }
    
    auto& AssetPaths = Pokemon::FBaseSettings::Get().GetDynamicAssetPaths();
    const auto &[ClassPath] = AssetPaths.MoveFunctionCodePackageName;
    auto &ClassPrefix = AssetPaths.MoveFunctionCodePrefix;
    auto FullPackage = FString::Format(TEXT("{0}/{1}{2}.{1}{2}_C"),
        { ClassPath, ClassPrefix, FunctionCode.ToString() });
    if (auto MoveClass = LoadObject<UClass>(nullptr, *FullPackage);
        MoveClass != nullptr && MoveClass->ImplementsInterface(UBattleMove::StaticClass())) {
        return MoveClass;
    }

    return UPokemonBattleMove::StaticClass();
}