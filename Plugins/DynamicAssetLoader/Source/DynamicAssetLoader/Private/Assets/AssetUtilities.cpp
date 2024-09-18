// "Unreal Pokémon" created by Retro & Chill.


#include "Assets/AssetUtilities.h"

FString UAssetUtilities::GetFullAssetName(FName Identifier, FStringView Prefix)  {
    return FString::Format(TEXT("{0}{1}"), {Prefix, Identifier.ToString()});
}