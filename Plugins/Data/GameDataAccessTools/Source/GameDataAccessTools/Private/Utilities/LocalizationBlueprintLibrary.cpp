// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/LocalizationBlueprintLibrary.h"

FString ULocalizationBlueprintLibrary::ToLocalizedString(const FText &Text)
{
    FString Output;
    FTextStringHelper::WriteToBuffer(Output, Text);
    return Output;
}

bool ULocalizationBlueprintLibrary::TryGetNamespace(const FText &Text, FString &OutNamespace)
{
    if (auto Namespace = FTextInspector::GetNamespace(Text); Namespace.IsSet())
    {
        OutNamespace = Namespace.GetValue();
        return true;
    }

    OutNamespace.Empty();
    return false;
}

bool ULocalizationBlueprintLibrary::TryGetKey(const FText &Text, FString &OutKey)
{
    if (auto Namespace = FTextInspector::GetKey(Text); Namespace.IsSet())
    {
        OutKey = Namespace.GetValue();
        return true;
    }

    OutKey.Empty();
    return false;
}