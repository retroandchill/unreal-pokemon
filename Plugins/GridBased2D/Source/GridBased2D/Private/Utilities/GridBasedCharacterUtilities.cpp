// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/GridBasedCharacterUtilities.h"
#include "CharacterMovementComponentAsync.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MathUtilities.h"

bool UGridBasedCharacterUtilities::InvalidFloor(ACharacter *Character, const FVector &TargetSquare,
                                                const UPrimitiveComponent *HitComponent) {
    if (HitComponent != nullptr && !CanStepUpOnComponent(Character, *HitComponent)) {
        return true;
    }

    auto Edge = FindLocationJustOffTileEdge(Character, TargetSquare);
    FFindFloorResult Result;
    Character->GetCharacterMovement()->FindFloor(Edge, Result, true);
    return !Result.bWalkableFloor;
}

bool UGridBasedCharacterUtilities::IsStandingNextToCliff(ACharacter *Character, const FVector &TargetSquare) {
    auto [Distance1, Component1] =
        PerformTraceToGround(Character, FindLocationJustOffTileEdge(Character, TargetSquare));
    auto [Distance2, Component2] =
        PerformTraceToGround(Character, FindLocationJustBeforeTileEdge(Character, TargetSquare));

    if (FMath::Abs(Distance1 - Distance2) > Character->GetCharacterMovement()->MaxStepHeight) {
        return true;
    }

    if (Component2 != nullptr) {
        return !CanStepUpOnComponent(Character, *Component2);
    }

    return false;
}

bool UGridBasedCharacterUtilities::CanStepUpOnComponent(ACharacter *Character, const UPrimitiveComponent &Component) {
    if (!Component.CanCharacterStepUp(Character)) {
        return false;
    }

    if (auto StaticMeshComponent = Cast<UStaticMeshComponent>(&Component); StaticMeshComponent != nullptr) {
        return StaticMeshComponent->GetWalkableSlopeOverride().WalkableSlopeBehavior != WalkableSlope_Unwalkable;
    }

    return true;
}

FVector UGridBasedCharacterUtilities::FindLocationJustOffTileEdge(ACharacter *Character, const FVector &TargetSquare) {
    auto Location = Character->GetActorLocation();
    auto MidPoint = UMathUtilities::Midpoint(TargetSquare, Location);
    auto Diff = TargetSquare - Location;
    Diff.Normalize();
    return MidPoint + Diff;
}

FVector UGridBasedCharacterUtilities::FindLocationJustBeforeTileEdge(ACharacter *Character,
                                                                     const FVector &TargetSquare) {
    auto Location = Character->GetActorLocation();
    auto MidPoint = UMathUtilities::Midpoint(TargetSquare, Location);
    auto Diff = TargetSquare - Location;
    Diff.Normalize();
    return MidPoint - Diff;
}

TPair<double, UPrimitiveComponent *> UGridBasedCharacterUtilities::PerformTraceToGround(ACharacter *Character,
                                                                                        const FVector &Position) {
    static constexpr double TraceMax = 100.0;
    FHitResult Result;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Character);
    auto Hit = Character->GetWorld()->LineTraceSingleByChannel(Result, Position, Position - FVector(0, 0, TraceMax),
                                                               ECC_Visibility, Params);
    return {Hit ? Result.Distance : TraceMax, Result.Component.Get()};
}