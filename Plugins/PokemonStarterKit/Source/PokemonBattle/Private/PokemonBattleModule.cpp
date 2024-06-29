#include "PokemonBattleModule.h"
#include "DataManager.h"
#include "GameplayTagsManager.h"
#include "Battle/Type.h"
#include "Battle/Types/TypeTags.h"

DEFINE_LOG_CATEGORY(LogBattle)

static void AddDynamicGameplayTag(UGameplayTagsManager& TagsManager, FStringView Format, FStringView DynamicName) {
    auto TagName = FString::Format(Format.GetData(), { DynamicName });
    TagsManager.AddNativeGameplayTag(*TagName);
}

void FPokemonBattleModule::StartupModule() {
    FCoreDelegates::OnPostEngineInit.AddRaw(this, &FPokemonBattleModule::OnPostEngineInit);
}

void FPokemonBattleModule::ShutdownModule() {
    // No shutdown code required
}

void FPokemonBattleModule::OnPostEngineInit() {
    using namespace Pokemon::Battle::Types;
    auto &GameplayTagManager = UGameplayTagsManager::Get();

    auto &DataManager = FDataManager::GetInstance();
    auto& TypeTable = DataManager.GetDataTable<FType>();
    TypeTable.ForEach([&GameplayTagManager](const FType& Type) {
        auto TypeNameString = Type.ID.ToString();
        // Add the tags for the types you attack from and defend from
        if (!Type.IsPseudoType) {
            AddDynamicGameplayTag(GameplayTagManager, AttackingTagsFormat, TypeNameString);
            AddDynamicGameplayTag(GameplayTagManager, DefendingTagsFormat, TypeNameString);
        }

        AddDynamicGameplayTag(GameplayTagManager, MoveTypeUserFormat, TypeNameString);
    });
}

IMPLEMENT_MODULE(FPokemonBattleModule, PokemonBattle)