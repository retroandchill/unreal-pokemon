// Fill out your copyright notice in the Description page of Project Settings.

#include "GridMapStaticMeshActor.h"

#include "TileSet.h"
#include "RetroLib/Functional/BindMethod.h"
#include "RetroLib/Optionals/Transform.h"
#include "RetroLib/Optionals/OrElseValue.h"

AGridMapStaticMeshActor::AGridMapStaticMeshActor(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer) {
    GetStaticMeshComponent()->SetGenerateOverlapEvents(true);
}

bool AGridMapStaticMeshActor::HasTerrainTag_Implementation(const FGameplayTag &Tag) const {
    // clang-format off
    return Retro::Optionals::OfNullable(TileSet) |
        Retro::Optionals::Transform(&UGridMapTileSet::TileTags) |
        Retro::Optionals::Transform(Retro::BindBack<&FGameplayTagContainer::HasTag>(Tag)) |
        Retro::Optionals::OrElseValue(false);
    // clang-format on
}

bool AGridMapStaticMeshActor::HasAnyTerrainTag_Implementation(const FGameplayTagContainer &TagsToCheck) const {
    // clang-format off
    return Retro::Optionals::OfNullable(TileSet) |
        Retro::Optionals::Transform(&UGridMapTileSet::TileTags) |
        Retro::Optionals::Transform(Retro::BindMethod<&FGameplayTagContainer::HasAny>(std::ref(TagsToCheck))) |
        Retro::Optionals::OrElseValue(false);
    // clang-format on
}

bool AGridMapStaticMeshActor::HasAllTerrainTags_Implementation(const FGameplayTagContainer &TagsToCheck) const {
    // clang-format off
    return Retro::Optionals::OfNullable(TileSet) |
        Retro::Optionals::Transform(&UGridMapTileSet::TileTags) |
        Retro::Optionals::Transform(Retro::BindMethod<&FGameplayTagContainer::HasAll>(std::ref(TagsToCheck))) |
        Retro::Optionals::OrElseValue(false);
    // clang-format on
}