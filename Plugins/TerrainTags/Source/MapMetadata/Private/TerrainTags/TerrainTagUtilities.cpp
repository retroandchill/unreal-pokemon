// "Unreal Pokémon" created by Retro & Chill.

#include "TerrainTags/TerrainTagUtilities.h"
#include "Components/CapsuleComponent.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Character.h"
#include "Ranges/Algorithm/FindFirst.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/Filter.h"
#include "Ranges/Views/FilterValid.h"
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
    auto Match = Overlaps | UE::Ranges::Map(&FOverlapResult::GetActor) | UE::Ranges::FilterValid |
                 UE::Ranges::Filter(&AActor::Implements<UTerrain>) |
                 UE::Ranges::Filter([&Tag](const AActor *A) { return ITerrain::Execute_HasTerrainTag(A, Tag); }) |
                 UE::Ranges::FindFirst;
    return Match.IsSet();
}

bool UTerrainTagUtilities::HasAnyTerrainTag(const UObject *WorldContext, const FGameplayTagContainer &Tags,
                                            const ACharacter *Character) {
    auto Overlaps = GetTerrainActors(WorldContext, Character);
    auto Match = Overlaps | UE::Ranges::Map(&FOverlapResult::GetActor) | UE::Ranges::FilterValid |
                 UE::Ranges::Filter(&AActor::Implements<UTerrain>) |
                 UE::Ranges::Filter([&Tags](const AActor *A) { return ITerrain::Execute_HasAnyTerrainTag(A, Tags); }) |
                 UE::Ranges::FindFirst;
    return Match.IsSet();
}

bool UTerrainTagUtilities::HasAllTerrainTags(const UObject *WorldContext, const FGameplayTagContainer &Tags,
                                             ACharacter *Character) {
    auto Overlaps = GetTerrainActors(WorldContext, Character);
    auto Match = Overlaps | UE::Ranges::Map(&FOverlapResult::GetActor) | UE::Ranges::FilterValid |
                 UE::Ranges::Filter(&AActor::Implements<UTerrain>) |
                 UE::Ranges::Filter([&Tags](const AActor *A) { return ITerrain::Execute_HasAllTerrainTags(A, Tags); }) |
                 UE::Ranges::FindFirst;
    return Match.IsSet();
}