// "Unreal Pokémon" created by Retro & Chill.

#include "Input/CommonInputExtensions.h"
#include "CommonUITypes.h"
#include "ICommonInputModule.h"

bool UCommonInputExtensions::IsEnhancedInputSupportEnabled()
{
    return CommonUI::IsEnhancedInputSupportEnabled();
}

UInputAction *UCommonInputExtensions::GetEnhancedInputBackAction()
{
    return ICommonInputModule::GetSettings().GetEnhancedInputBackAction();
}

FDataTableRowHandle UCommonInputExtensions::GetDefaultBackAction()
{
    return ICommonInputModule::GetSettings().GetDefaultBackAction();
}