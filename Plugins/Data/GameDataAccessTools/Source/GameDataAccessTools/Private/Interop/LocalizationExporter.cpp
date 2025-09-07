// "Unreal Pokémon" created by Retro & Chill.

#include "Interop/LocalizationExporter.h"

void ULocalizationExporter::CreateLocalizedText(const TCHAR *Namespace, const TCHAR *Key, const TCHAR *DefaultValue,
                                                FText &OutText)
{
    OutText = FText::AsLocalizable_Advanced(Namespace, Key, DefaultValue);
}