// "Unreal Pokémon" created by Retro & Chill.

#include "Input/ActionRouterExtensions.h"
#include "Input/CommonUIActionRouterBase.h"
#include "Input/CSBindUIActionArgs.h"

FUIActionBindingHandle UActionRouterExtensions::RegisterUIActionBinding(UCommonUIActionRouterBase *ActionRouter,
                                                                        const UWidget *Widget,
                                                                        const FCSBindUIActionArgs &BindActionArgs)
{
    return ActionRouter->RegisterUIActionBinding(*Widget, *BindActionArgs.GetArgs());
}