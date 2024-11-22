// "Unreal Pokémon" created by Retro & Chill.

#include "TerrainTags/TerrainTagUtilities.h"
#include "Components/CapsuleComponent.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Character.h"
#include "Ranges/Algorithm/AnyOf.h"
#include "Ranges/Algorithm/FindFirst.h"
#include "Ranges/Casting/InstanceOf.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/Filter.h"
#include "Ranges/Views/Map.h"
#include "TerrainTags/Terrain.h"

constexpr float CapsuleOffset = 5.f;

static TArray<FOverlapResult> GetTerrainActors(const UObject *WorldContext, const ACharacter *Character) {
    auto World = WorldContext->GetWorld();
    check(World != nullptr)

    auto Capsule = Character->GetCapsuleComponent();
    FCollisionShape CollisionSphere;
    CollisionSphere.SetCapsule(Capsule->GetScaledCapsuleRadius(), Capsule->GetScaledCapsuleHalfHeight());
    FCollisionQueryParams Parameters;
    Parameters.AddIgnoredActor(Character);

    TArray<FOverlapResult> Result;
    World->OverlapMultiByChannel(Result, Character->GetActorLocation() - FVector(0, 0, CapsuleOffset),
                                 Character->GetActorRotation().Quaternion(), ECC_WorldDynamic, CollisionSphere);

    return Result;
}

bool UTerrainTagUtilities::HasTerrainTag(const UObject *WorldContext, const FGameplayTag &Tag,
                                         const ACharacter *Character) {
    auto Overlaps = GetTerrainActors(WorldContext, Character);
    // clang-format off
    return Overlaps |
           UE::Ranges::Map(&FOverlapResult::GetActor) |
           UE::Ranges::Filter(UE::Ranges::InstanceOf<ITerrain>) |
           UE::Ranges::AnyOf([&Tag](const AActor *A) {
               return ITerrain::Execute_HasTerrainTag(A, Tag);
           });
    // clang-format on
}

bool UTerrainTagUtilities::HasAnyTerrainTag(const UObject *WorldContext, const FGameplayTagContainer &Tags,
                                            const ACharacter *Character) {
    auto Overlaps = GetTerrainActors(WorldContext, Character);
    // clang-format off
    return Overlaps |
           UE::Ranges::Map(&FOverlapResult::GetActor) |
           UE::Ranges::Filter(UE::Ranges::InstanceOf<ITerrain>) |
           UE::Ranges::AnyOf([&Tags](const AActor *A) {
               return ITerrain::Execute_HasAnyTerrainTag(A, Tags);
           });
    // clang-format on
}

bool UTerrainTagUtilities::HasAllTerrainTags(const UObject *WorldContext, const FGameplayTagContainer &Tags,
                                             ACharacter *Character) {
    auto Overlaps = GetTerrainActors(WorldContext, Character);
    // clang-format off
    return Overlaps |
           UE::Ranges::Map(&FOverlapResult::GetActor) |
           UE::Ranges::Filter(UE::Ranges::InstanceOf<ITerrain>) |
           UE::Ranges::AnyOf([&Tags](const AActor *A) {
               return ITerrain::Execute_HasAllTerrainTags(A, Tags);
           });
    // clang-format on
}