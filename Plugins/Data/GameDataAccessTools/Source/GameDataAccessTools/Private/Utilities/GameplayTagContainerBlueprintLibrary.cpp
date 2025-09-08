// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/GameplayTagContainerBlueprintLibrary.h"

FGameplayTagContainer UGameplayTagContainerBlueprintLibrary::Filter(const FGameplayTagContainerView Self,
                                                                    const FGameplayTagContainer &OtherContainer)
{
    return Self.Get().Filter(OtherContainer);
}

FGameplayTagContainer UGameplayTagContainerBlueprintLibrary::FilterExact(const FGameplayTagContainerView Self,
                                                                         const FGameplayTagContainer &OtherContainer)
{
    return Self.Get().FilterExact(OtherContainer);
}

FGameplayTagContainer UGameplayTagContainerBlueprintLibrary::FilterView(const FGameplayTagContainerView Self,
                                                                        const FGameplayTagContainerView OtherContainer)
{
    return Self.Get().Filter(OtherContainer.Get());
}

FGameplayTagContainer UGameplayTagContainerBlueprintLibrary::FilterViewExact(
    const FGameplayTagContainerView Self, const FGameplayTagContainerView OtherContainer)
{
    return Self.Get().FilterExact(OtherContainer.Get());
}

FGameplayTag UGameplayTagContainerBlueprintLibrary::First(const FGameplayTagContainerView Self)
{
    return Self.Get().First();
}

FGameplayTag UGameplayTagContainerBlueprintLibrary::GetByIndex(const FGameplayTagContainerView Self, const int32 Index)
{
    return Self.Get().GetByIndex(Index);
}

FGameplayTagArrayView UGameplayTagContainerBlueprintLibrary::GetGameplayTagArray(FGameplayTagContainerView Self)
{
    return FGameplayTagArrayView(Self.Get().GetGameplayTagArray());
}

FGameplayTagContainer UGameplayTagContainerBlueprintLibrary::GetGameplayTagParents(FGameplayTagContainerView Self)
{
    return Self.Get().GetGameplayTagParents();
}

bool UGameplayTagContainerBlueprintLibrary::HasAll(const FGameplayTagContainerView Self,
                                                   const FGameplayTagContainer &ContainerToCheck)
{
    return Self.Get().HasAll(ContainerToCheck);
}

bool UGameplayTagContainerBlueprintLibrary::HasAllExact(const FGameplayTagContainerView Self,
                                                        const FGameplayTagContainer &ContainerToCheck)
{
    return Self.Get().HasAllExact(ContainerToCheck);
}

bool UGameplayTagContainerBlueprintLibrary::HasAny(const FGameplayTagContainerView Self,
                                                   const FGameplayTagContainer &ContainerToCheck)
{
    return Self.Get().HasAny(ContainerToCheck);
}

bool UGameplayTagContainerBlueprintLibrary::HasAllInView(const FGameplayTagContainerView Self,
                                                         const FGameplayTagContainerView ContainerToCheck)
{
    return Self.Get().HasAll(ContainerToCheck.Get());
}

bool UGameplayTagContainerBlueprintLibrary::HasAllExactInView(const FGameplayTagContainerView Self,
                                                              const FGameplayTagContainerView ContainerToCheck)
{
    return Self.Get().HasAllExact(ContainerToCheck.Get());
}

bool UGameplayTagContainerBlueprintLibrary::HasAnyInView(const FGameplayTagContainerView Self,
                                                         const FGameplayTagContainerView ContainerToCheck)
{
    return Self.Get().HasAny(ContainerToCheck.Get());
}

bool UGameplayTagContainerBlueprintLibrary::HasTag(const FGameplayTagContainerView Self, const FGameplayTag TagToCheck)
{
    return Self.Get().HasTag(TagToCheck);
}

bool UGameplayTagContainerBlueprintLibrary::HasTagExact(const FGameplayTagContainerView Self,
                                                        const FGameplayTag TagToCheck)
{
    return Self.Get().HasTagExact(TagToCheck);
}

bool UGameplayTagContainerBlueprintLibrary::IsEmpty(const FGameplayTagContainerView Self)
{
    return Self.Get().IsEmpty();
}

bool UGameplayTagContainerBlueprintLibrary::IsValid(const FGameplayTagContainerView Self)
{
    return Self.Get().IsValid();
}

bool UGameplayTagContainerBlueprintLibrary::IsValidIndex(const FGameplayTagContainerView Self, const int32 Index)
{
    return Self.Get().IsValidIndex(Index);
}

FGameplayTag UGameplayTagContainerBlueprintLibrary::Last(const FGameplayTagContainerView Self)
{
    return Self.Get().Last();
}

bool UGameplayTagContainerBlueprintLibrary::MatchesQuery(const FGameplayTagContainerView Self,
                                                         const FGameplayTagQuery &Query)
{
    return Self.Get().MatchesQuery(Query);
}

int32 UGameplayTagContainerBlueprintLibrary::Num(const FGameplayTagContainerView Self)
{
    return Self.Get().Num();
}