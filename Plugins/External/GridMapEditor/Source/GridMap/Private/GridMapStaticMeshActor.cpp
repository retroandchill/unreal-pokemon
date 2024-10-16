// Fill out your copyright notice in the Description page of Project Settings.

#include "GridMapStaticMeshActor.h"
#include "Ranges/Optional/Map.h"
#include "Ranges/Optional/OptionalRef.h"
#include "Ranges/Optional/OrElse.h"
#include "TileSet.h"

AGridMapStaticMeshActor::AGridMapStaticMeshActor(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer) {
    GetStaticMeshComponent()->SetGenerateOverlapEvents(true);
}

bool AGridMapStaticMeshActor::HasTerrainTag_Implementation(const FGameplayTag &Tag) const {
    // clang-format off
    return UE::Optionals::OfNullable(TileSet) |
        UE::Optionals::Map(&UGridMapTileSet::TileTags) |
        UE::Optionals::Map(&FGameplayTagContainer::HasTag, Tag) |
        UE::Optionals::OrElse(false);
    // clang-format on
}

bool AGridMapStaticMeshActor::HasAnyTerrainTag_Implementation(const FGameplayTagContainer &TagsToCheck) const {
    // clang-format off
    return UE::Optionals::OfNullable(TileSet) |
        UE::Optionals::Map(&UGridMapTileSet::TileTags) |
        UE::Optionals::Map(&FGameplayTagContainer::HasAny, TagsToCheck) |
        UE::Optionals::OrElse(false);
    // clang-format on
}

bool AGridMapStaticMeshActor::HasAllTerrainTags_Implementation(const FGameplayTagContainer &TagsToCheck) const {
    // clang-format off
    return UE::Optionals::OfNullable(TileSet) |
        UE::Optionals::Map(&UGridMapTileSet::TileTags) |
        UE::Optionals::Map(&FGameplayTagContainer::HasAll, TagsToCheck) |
        UE::Optionals::OrElse(false);
    // clang-format on
}