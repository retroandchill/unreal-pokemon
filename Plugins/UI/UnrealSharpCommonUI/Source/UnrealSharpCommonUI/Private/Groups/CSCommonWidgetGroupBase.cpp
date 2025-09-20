// "Unreal Pokémon" created by Retro & Chill.

#include "Groups/CSCommonWidgetGroupBase.h"

void UCSCommonWidgetGroupBase::OnWidgetAdded(UWidget *NewWidget)
{
    Super::OnWidgetAdded(NewWidget);
    K2_OnWidgetAdded(NewWidget);
}

void UCSCommonWidgetGroupBase::OnWidgetRemoved(UWidget *RemovedWidget)
{
    Super::OnWidgetRemoved(RemovedWidget);
    K2_OnWidgetRemoved(RemovedWidget);
}

void UCSCommonWidgetGroupBase::OnRemoveAll()
{
    Super::OnRemoveAll();
    K2_OnRemoveAll();
}