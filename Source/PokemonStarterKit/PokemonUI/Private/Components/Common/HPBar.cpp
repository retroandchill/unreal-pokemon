// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Common/HPBar.h"

TSharedRef<SWidget> UHPBar::RebuildWidget()
{
    bSetUp = true;
    UpdateBarMaterial();
    return Super::RebuildWidget();
}

#if WITH_EDITOR
void UHPBar::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    UpdateBarMaterial();
}
#endif

void UHPBar::SynchronizeProperties()
{
    UpdateBarMaterial();
    Super::SynchronizeProperties();
}

void UHPBar::Tick(float DeltaTime)
{
    if (!bSetUp)
    {
        return;
    }

    float CurrentPercent = GetPercent();
    if (CurrentPercent == PreviousPercent)
    {
        return;
    }

    PreviousPercent = CurrentPercent;
    UpdateBarMaterial();
}

TStatId UHPBar::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UHPBar, STATGROUP_Tickables)
}

void UHPBar::UpdateBarMaterial()
{
    int32 State = INDEX_NONE;
    for (int32 i = 0; i < PercentThresholds.Num(); i++)
    {
        if (GetPercent() > PercentThresholds[i].Threshold)
        {
            break;
        }

        State = i;
    }

    if (CurrentState.IsSet() && *CurrentState == State)
    {
        return;
    }

    auto Style = GetWidgetStyle();
    if (State == INDEX_NONE)
    {
        Style.FillImage = DefaultStyle;
    }
    else
    {
        Style.FillImage = PercentThresholds[State].Style;
    }
    SetWidgetStyle(Style);
    CurrentState.Emplace(State);
}