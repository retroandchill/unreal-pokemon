// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeView.h"
#include "UObject/Object.h"

#include "GameplayTagContainerBlueprintLibrary.generated.h"

struct FGameplayTagContainer;

USTRUCT(meta = (InternalType))
struct FGameplayTagArrayView
#if CPP
    : TNativeView<TArray<FGameplayTag>>
#endif
{
    GENERATED_BODY()

    using TNativeView::TNativeView;
};

USTRUCT(meta = (InternalType))
struct FGameplayTagContainerView
#if CPP
    : TNativeView<FGameplayTagContainer>
#endif
{
    GENERATED_BODY()

    using TNativeView::TNativeView;
};

/**
 *
 */
UCLASS(meta = (InternalType))
class GAMEDATAACCESSTOOLS_API UGameplayTagContainerBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    UFUNCTION(meta = (ScriptMethod))
    static FGameplayTagContainer Filter(FGameplayTagContainerView Self, const FGameplayTagContainer &OtherContainer);

    UFUNCTION(meta = (ScriptMethod))
    static FGameplayTagContainer FilterExact(FGameplayTagContainerView Self,
                                             const FGameplayTagContainer &OtherContainer);

    UFUNCTION(meta = (ScriptMethod))
    static FGameplayTagContainer FilterView(FGameplayTagContainerView Self, FGameplayTagContainerView OtherContainer);

    UFUNCTION(meta = (ScriptMethod))
    static FGameplayTagContainer FilterViewExact(FGameplayTagContainerView Self,
                                                 FGameplayTagContainerView OtherContainer);

    UFUNCTION(meta = (ScriptMethod))
    static FGameplayTag First(FGameplayTagContainerView Self);

    UFUNCTION(meta = (ScriptMethod))
    static FGameplayTag GetByIndex(FGameplayTagContainerView Self, int32 Index);

    UFUNCTION(meta = (ScriptMethod))
    static FGameplayTagArrayView GetGameplayTagArray(FGameplayTagContainerView Self);

    UFUNCTION(meta = (ScriptMethod))
    static FGameplayTagContainer GetGameplayTagParents(FGameplayTagContainerView Self);

    UFUNCTION(meta = (ScriptMethod))
    static bool HasAll(FGameplayTagContainerView Self, const FGameplayTagContainer &ContainerToCheck);

    UFUNCTION(meta = (ScriptMethod))
    static bool HasAllExact(FGameplayTagContainerView Self, const FGameplayTagContainer &ContainerToCheck);

    UFUNCTION(meta = (ScriptMethod))
    static bool HasAny(FGameplayTagContainerView Self, const FGameplayTagContainer &ContainerToCheck);

    UFUNCTION(meta = (ScriptMethod))
    static bool HasAllInView(FGameplayTagContainerView Self, FGameplayTagContainerView ContainerToCheck);

    UFUNCTION(meta = (ScriptMethod))
    static bool HasAllExactInView(FGameplayTagContainerView Self, FGameplayTagContainerView ContainerToCheck);

    UFUNCTION(meta = (ScriptMethod))
    static bool HasAnyInView(FGameplayTagContainerView Self, FGameplayTagContainerView ContainerToCheck);

    UFUNCTION(meta = (ScriptMethod))
    static bool HasTag(FGameplayTagContainerView Self, FGameplayTag TagToCheck);

    UFUNCTION(meta = (ScriptMethod))
    static bool HasTagExact(FGameplayTagContainerView Self, FGameplayTag TagToCheck);

    UFUNCTION(meta = (ScriptMethod))
    static bool IsEmpty(FGameplayTagContainerView Self);

    UFUNCTION(meta = (ScriptMethod))
    static bool IsValid(FGameplayTagContainerView Self);

    UFUNCTION(meta = (ScriptMethod))
    static bool IsValidIndex(FGameplayTagContainerView Self, int32 Index);

    UFUNCTION(meta = (ScriptMethod))
    static FGameplayTag Last(FGameplayTagContainerView Self);

    UFUNCTION(meta = (ScriptMethod))
    static bool MatchesQuery(FGameplayTagContainerView Self, const FGameplayTagQuery &Query);

    UFUNCTION(meta = (ScriptMethod))
    static int32 Num(FGameplayTagContainerView Self);
};
