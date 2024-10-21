// "Unreal Pokémon" created by Retro & Chill.


#include "Assets/AssetClass.h"


namespace UE::Assets {
    FAssetClassRegistry & FAssetClassRegistry::Get() {
        static FAssetClassRegistry Instance;
        return Instance;
    }

    TOptional<IAssetClassRegistration &> FAssetClassRegistry::GetAssetClassRegistration(FName Key) const {
        return Optionals::OfNullable(AssetClassRegistry.Find(Key)) |
            Optionals::Map(&TUniquePtr<IAssetClassRegistration>::Get);
    }
}