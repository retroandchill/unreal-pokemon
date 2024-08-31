// "Unreal Pokémon" created by Retro & Chill.


#include "TerrainTagUtilities.h"
#include "Terrain.h"
#include "Engine/OverlapResult.h"
#include "Ranges/Algorithm/FindFirst.h"
#include "Ranges/Views/Map.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/Filter.h"

static auto GetTerrainActors(const UObject* WorldContext, const FVector &Position, float Radius) {
    auto World = WorldContext->GetWorld();
    check(World != nullptr)

    FCollisionShape CollisionSphere;
    CollisionSphere.SetSphere(Radius);

    TArray<FOverlapResult> Result;
    World->OverlapMultiByChannel(Result, Position, FQuat(), ECC_WorldDynamic, CollisionSphere);

    return Result |
        UE::Ranges::Map(&FOverlapResult::GetActor) |
        UE::Ranges::Filter(&AActor::Implements<UTerrain>);
}

bool UTerrainTagUtilities::HasTerrainTag(const UObject* WorldContext, const FGameplayTag &Tag,
                                         const FVector &Position, float Radius) {
    auto Match = GetTerrainActors(WorldContext, Position, Radius) |
        UE::Ranges::Filter([&Tag](const AActor* A) { return ITerrain::Execute_HasTerrainTag(A, Tag); }) |
        UE::Ranges::FindFirst;
    return Match.IsSet();
}

bool UTerrainTagUtilities::HasAnyTerrainTag(const UObject *WorldContext, const FGameplayTagContainer &Tags,
    const FVector &Position, float Radius) {
    auto Match = GetTerrainActors(WorldContext, Position, Radius) |
        UE::Ranges::Filter([&Tags](const AActor* A) { return ITerrain::Execute_HasAnyTerrainTag(A, Tags); }) |
        UE::Ranges::FindFirst;
    return Match.IsSet();
}

bool UTerrainTagUtilities::HasAllTerrainTags(const UObject *WorldContext, const FGameplayTagContainer &Tags,
    const FVector &Position, float Radius) {
    auto Match = GetTerrainActors(WorldContext, Position, Radius) |
        UE::Ranges::Filter([&Tags](const AActor* A) { return ITerrain::Execute_HasAllTerrainTags(A, Tags); }) |
        UE::Ranges::FindFirst;
    return Match.IsSet();
}