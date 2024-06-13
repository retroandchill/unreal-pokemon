// "Unreal Pokémon" created by Retro & Chill.


#include "Components/HPBar.h"

TSharedRef<SWidget> UHPBar::RebuildWidget() {
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

void UHPBar::UpdateBarMaterial() {
    bool bMaterialChanged = false;
    if (BarMaterial == nullptr || BarMaterial->GetBaseMaterial() != BaseMaterial) {
        BarMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
        bMaterialChanged = true;
    }
    uint32 State = 0;
    for (uint32 i = 0; i < PercentThresholds.Num(); i++) {
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