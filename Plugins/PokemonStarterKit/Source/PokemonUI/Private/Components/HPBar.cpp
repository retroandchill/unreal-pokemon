// "Unreal Pokémon" created by Retro & Chill.

#include "Components/HPBar.h"

TSharedRef<SWidget> UHPBar::RebuildWidget() {
    bSetUp = true;
    UpdateBarMaterial();
    return Super::RebuildWidget();
}

#if WITH_EDITOR
void UHPBar::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) {
    Super::PostEditChangeProperty(PropertyChangedEvent);
    UpdateBarMaterial();
}
#endif

void UHPBar::SynchronizeProperties() {
    UpdateBarMaterial();
    Super::SynchronizeProperties();
}

void UHPBar::Tick(float DeltaTime) {
    if (!bSetUp) {
        return;
    }

    float CurrentPercent = GetPercent();
    if (CurrentPercent == PreviousPercent) {
        return;
    }

    PreviousPercent = CurrentPercent;
    UpdateBarMaterial();
}

TStatId UHPBar::GetStatId() const {
    RETURN_QUICK_DECLARE_CYCLE_STAT(UHPBar, STATGROUP_Tickables)
}

void UHPBar::UpdateBarMaterial() {
    bool bMaterialChanged = false;
    if (BarMaterial == nullptr || BarMaterial->GetBaseMaterial() != BaseMaterial) {
        BarMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
        bMaterialChanged = true;
    }
    uint32 State = 0;
    for (int32 i = 0; i < PercentThresholds.Num(); i++) {
        if (GetPercent() > PercentThresholds[i]) {
            break;
        }

        State++;
    }

    BarMaterial->SetScalarParameterValue(StateParameterName, static_cast<float>(State));
    if (bMaterialChanged) {
        auto Style = GetWidgetStyle();
        Style.FillImage.SetResourceObject(BarMaterial);
        SetWidgetStyle(Style);
    }
}