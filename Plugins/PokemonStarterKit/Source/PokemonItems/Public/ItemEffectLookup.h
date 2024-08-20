// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "DynamicAssetLoadingSettings.h"

class UFieldItemEffect;

namespace Pokemon::Items {

    template <typename T>
        requires std::is_base_of_v<UFieldItemEffect, T>
    TSubclassOf<T> LookupFieldItemEffect(FName ID) {
        if (ID.IsNone()) {
            return nullptr;
        }

        decltype(UDynamicAssetLoadingSettings::FieldItemEffectPrefix) Effect;
        auto &AssetPaths = *GetDefault<UDynamicAssetLoadingSettings>();
        const auto &[ClassPath] = AssetPaths.FieldItemEffectPackageName;
        auto &ClassPrefix = AssetPaths.FieldItemEffectPrefix;
        auto FullPackage = FString::Format(TEXT("{0}/{1}{2}.{1}{2}_C"), {ClassPath, ClassPrefix, ID.ToString()});
        if (auto ItemClass = LoadObject<UClass>(nullptr, *FullPackage);
            ItemClass != nullptr && ItemClass->IsChildOf<T>()) {
            return ItemClass;
        }

        return nullptr;
    }

} // namespace Pokemon::Items