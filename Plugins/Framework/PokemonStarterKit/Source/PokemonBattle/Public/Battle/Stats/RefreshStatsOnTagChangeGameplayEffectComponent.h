// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "AttributeSet.h"
#include "GameplayEffectComponent.h"
#include "GameplayTagContainer.h"

#include "RefreshStatsOnTagChangeGameplayEffectComponent.generated.h"

struct FGameplayEffectRemovalInfo;

/**
 * Container of the stat affecting attributes
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FStatUpdateInfo {
    GENERATED_BODY()

    /**
     * The attribute that is affected by the tag change
     */
    UPROPERTY(EditAnywhere, Category = "Battle|Stats")
    FGameplayAttribute AffectedAttribute;

    /**
     * The set of changed tags
     */
    UPROPERTY(EditAnywhere, Category = "Battle|Stats")
    FGameplayTagContainer AffectingTags;
};

/**
 * Mark stat values as dirty upon a stat change
 */
UCLASS(DisplayName = "Refresh Stats on Tag Change")
class POKEMONBATTLE_API URefreshStatsOnTagChangeGameplayEffectComponent : public UGameplayEffectComponent {
    GENERATED_BODY()

  public:
    bool OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer &ActiveGEContainer,
                                     FActiveGameplayEffect &ActiveGE) const override;

  private:
    /**
     * Called when a gameplay tag is changed, used to force a refresh of the owned stat
     * @param GameplayTag The gameplay tag that changed
     * @param NewCount The new tag count
     * @param ActiveGEHandle The active handle for the effect
     */
    void OnTagChanged(const FGameplayTag GameplayTag, int32 NewCount, FActiveGameplayEffectHandle ActiveGEHandle) const;

    /**
     * Called when this effect is removed in order to clean up any bound delegates
     * @param GERemovalInfo The remove info for the attribute
     * @param ASC The ability system component that needs to be unbound from
     * @param AllBoundEvents The bound events to release
     */
    void OnGameplayEffectRemoved(const FGameplayEffectRemovalInfo &GERemovalInfo, UAbilitySystemComponent *ASC,
                                 TArray<TTuple<FGameplayTag, FDelegateHandle>> AllBoundEvents) const;

    /**
     * The stats that need to have update data
     */
    UPROPERTY(EditAnywhere)
    TArray<FStatUpdateInfo> StatsToUpdate;
};
