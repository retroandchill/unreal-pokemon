// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Actions/BattleActionUseItem.h"
#include "DataManager.h"
#include "PokemonBattleSettings.h"
#include "Bag/Item.h"
#include "Battle/Battlers/Battler.h"

FItemTarget::FItemTarget(TWeakInterfacePtr<IBattler> &&Battler) {
    Data.Set<TWeakInterfacePtr<IBattler>>(MoveTemp(Battler));
}

FItemTarget::FItemTarget(TWeakInterfacePtr<IBattleMove> &&Move) {
    Data.Set<TWeakInterfacePtr<IBattleMove>>(MoveTemp(Move));
}

FItemTarget::FItemTarget(FTargetWithIndex &&Target) {
    Data.Set<FTargetWithIndex>(MoveTemp(Target));
}

FBattleActionUseItem::FBattleActionUseItem(const TScriptInterface<IBattler> &Battler, FName ItemID,
                                           FItemTarget &&ItemTarget) : FBattleActionBase(Battler), ItemID(ItemID),
                                                                       ItemTarget(MoveTemp(ItemTarget)) {
}

FString FBattleActionUseItem::GetReferencerName() const {
    return TEXT("FBattleActionUseItem");
}

int32 FBattleActionUseItem::GetPriority() const {
    return GetDefault<UPokemonBattleSettings>()->SwitchPriority;
}

FText FBattleActionUseItem::GetActionMessage() const {
    auto &ItemData = FDataManager::GetInstance().GetDataTable<FItem>().GetDataChecked(ItemID);
    return FText::Format(FText::FromStringView(TEXT("{0} used {1}!")), {
        GetBattler()->GetWrappedPokemon()->GetCurrentHandler()->GetTrainerName(),
        ItemData.RealName
    });
}

FGameplayAbilitySpecHandle FBattleActionUseItem::ActivateAbility() {
    return FGameplayAbilitySpecHandle();
}