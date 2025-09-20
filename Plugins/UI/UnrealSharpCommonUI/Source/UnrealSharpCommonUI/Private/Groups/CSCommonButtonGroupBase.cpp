// "Unreal Pokémon" created by Retro & Chill.

#include "Groups/CSCommonButtonGroupBase.h"

void UCSCommonButtonGroupBase::OnWidgetAdded(UWidget *NewWidget)
{
    Super::OnWidgetAdded(NewWidget);
    K2_OnWidgetAdded(CastChecked<UCommonButtonBase>(NewWidget));
}

void UCSCommonButtonGroupBase::OnWidgetRemoved(UWidget *RemovedWidget)
{
    Super::OnWidgetRemoved(RemovedWidget);
    K2_OnWidgetRemoved(CastChecked<UCommonButtonBase>(RemovedWidget));
}

void UCSCommonButtonGroupBase::OnRemoveAll()
{
    Super::OnRemoveAll();
    K2_OnRemoveAll();
}