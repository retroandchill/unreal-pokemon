// "Unreal Pokémon" created by Retro & Chill.

#include "Components/CharacterBillboardAnimationComponent.h"
#include "Components/MaterialBillboardComponent.h"

void UCharacterBillboardAnimationComponent::UpdateDirection(EFacingDirection Direction) {
    if (MaterialInstance == nullptr) {
        SetUpMaterialInstance();
    }
    MaterialInstance->SetScalarParameterValue(DirectionPropertyName, static_cast<float>(Direction));
    if (!CurrentDirection.IsSet() || CurrentDirection.GetValue() != Direction) {
        MaterialInstance->SetScalarParameterValue(StartTimePropertyName,
                                                  static_cast<float>(GetWorld()->GetRealTimeSeconds()));
        PlayingTime.Emplace(0.0);
        CurrentDirection.Emplace(Direction);
    }
}

bool UCharacterBillboardAnimationComponent::IsMoveAnimationPlaying() const {
    return MaterialInstance != nullptr && bIsPlaying;
}

void UCharacterBillboardAnimationComponent::StartMoveAnimation() {
    if (MaterialInstance == nullptr) {
        SetUpMaterialInstance();
    }

    MaterialInstance->SetScalarParameterValue(FrameRatePropertyName, OriginalFrameRate);
    if (!PlayingTime.IsSet()) {
        MaterialInstance->SetScalarParameterValue(StartTimePropertyName,
                                                  static_cast<float>(GetWorld()->GetRealTimeSeconds()));
        PlayingTime.Emplace(0.0);
    }
}

bool UCharacterBillboardAnimationComponent::CanStopMoving() const {
    if (!PlayingTime.IsSet()) {
        return false;
    }

    float FrameRate = MaterialInstance->K2_GetScalarParameterValue(FrameRatePropertyName);
    int32 TotalFramesPlayed = FMath::FloorToInt32(PlayingTime.GetValue() * FrameRate);
    int32 CurrentFrame = TotalFramesPlayed % TotalFrames;
    return ValidStopFrames.Contains(CurrentFrame);
}

void UCharacterBillboardAnimationComponent::StopMoveAnimation() {
    if (MaterialInstance == nullptr) {
        SetUpMaterialInstance();
    }
    MaterialInstance->SetScalarParameterValue(FrameRatePropertyName, 0);
    PlayingTime.Reset();
}

void UCharacterBillboardAnimationComponent::BeginPlay() {
    Super::BeginPlay();

    SetUpMaterialInstance();
    if (BillboardComponent == nullptr) {
        return;
    }
    BillboardComponent->Elements.Empty();
    BillboardComponent->AddElement(MaterialInstance, nullptr, false,
                                   static_cast<float>(SourceTexture->GetSizeY()) / (4 * 2),
                                   static_cast<float>(SourceTexture->GetSizeX()) / (4 * 2), nullptr);
}

void UCharacterBillboardAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                          FActorComponentTickFunction *ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (PlayingTime.IsSet()) {
        PlayingTime.GetValue() += DeltaTime;
    }
}

void UCharacterBillboardAnimationComponent::SetUpMaterialInstance() {
    if (BaseMaterial == nullptr || SourceTexture == nullptr) {
        return;
    }

    MaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, this);
    OriginalFrameRate = MaterialInstance->K2_GetScalarParameterValue(FrameRatePropertyName);
    MaterialInstance->SetTextureParameterValue(SourceTexturePropertyName, SourceTexture);
    MaterialInstance->SetScalarParameterValue(FrameRatePropertyName, 0);

    if (BillboardComponent == nullptr) {
        return;
    }
    BillboardComponent->Elements.Empty();
    BillboardComponent->AddElement(MaterialInstance, nullptr, false, SourceTexture->GetSizeY() / (4 * 2),
                                   SourceTexture->GetSizeX() / (4 * 2), nullptr);
}

UMaterialBillboardComponent *UCharacterBillboardAnimationComponent::GetBillboardComponent() const {
    return BillboardComponent;
}

void UCharacterBillboardAnimationComponent::SetBillboardComponent(UMaterialBillboardComponent *NewBillboardComponent) {
    BillboardComponent = NewBillboardComponent;
}

UMaterialInstanceDynamic *UCharacterBillboardAnimationComponent::GetMaterialInstance() const {
    return MaterialInstance;
}

void UCharacterBillboardAnimationComponent::SetMaterialInstance(UMaterialInstanceDynamic *NewMaterialInstance) {
    MaterialInstance = NewMaterialInstance;
}
