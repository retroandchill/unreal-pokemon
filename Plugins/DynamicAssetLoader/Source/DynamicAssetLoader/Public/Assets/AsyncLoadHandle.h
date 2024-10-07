// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AssetLoadingSettings.h"
#include "Engine/AssetManager.h"
#include "Ranges/Functional/Delegates.h"
#include "Ranges/Optional/Construct.h"
#include "Ranges/Optional/Filter.h"

namespace UE::Assets {
    /**
     * The handle used for async loading of a single asset
     */
    template <typename T>
        requires AssetClassType<T>
    class TAsyncLoadHandle {
    public:
        using AssetType = std::conditional_t<Ranges::VariantObjectStruct<T>, T, T&>;
        DECLARE_MULTICAST_DELEGATE_OneParam(FOnAssetLoaded, TOptional<AssetType>)

    private:
        struct FPrivateToken { explicit FPrivateToken() = default; };

    public:
        explicit TAsyncLoadHandle(FPrivateToken) {}
        
        template <typename... A>
            requires std::constructible_from<FSoftObjectPath, A...>
        static TSharedRef<TAsyncLoadHandle> Create(A&&... Args) {
            auto Ret = MakeShared<TAsyncLoadHandle>(FPrivateToken{});
            auto &StreamableManager = UAssetManager::GetStreamableManager();
            Ret->Handle = StreamableManager.RequestAsyncLoad(FSoftObjectPath(Forward<A>(Args)...),
                FStreamableDelegate::CreateSP(Ret, &TAsyncLoadHandle::OnAssetLoaded));
            return Ret;
        }
        
        bool IsLoaded() const {
            return Handle->HasLoadCompleted();
        }

        template <typename F, typename... A>
            requires std::invocable<F, TOptional<AssetType>, A...>
        void OnLoadComplete(F&& Functor, A&&... Args) {
            if (IsLoaded()) {
                std::invoke(Forward<F>(Functor), Asset, Forward(Args)...);
            } else {
                Ranges::AddToDelegate(OnLoadCompleteDelegate, Forward<F>(Functor), Forward(Args)...);
            }
        }

        template <typename F, typename O, typename... A>
            requires std::invocable<F, O, TOptional<AssetType>, A...>
        void OnLoadComplete(O&& Object, F&& Functor, A&&... Args) {
            if (IsLoaded()) {
                std::invoke(Forward<F>(Functor), Forward<O>(Object), Asset, Forward(Args)...);
            } else {
                Ranges::AddToDelegate(OnLoadCompleteDelegate, Forward<F>(Functor), Forward<O>(Object), Forward(Args)...);
            }
        }

        TPair<const TOptional<AssetType> &, EAsyncPackageState::Type> GetAsset(float Timeout = 0.f) const {
            auto Result = Handle->WaitUntilComplete(Timeout);
            return { Asset, Result };
        }
        
        
    private:
        void OnAssetLoaded() {
            if constexpr (Ranges::VariantObjectStruct<T>) {
                // clang-format off
                Asset = Optionals::OfNullable(Handle->GetLoadedAsset()) |
                    Optionals::Filter(&T::IsValidType) |
                    Optionals::Construct<T>();
                // clang-format on
            } else if constexpr (std::is_same_v<T, UObject>) {
                Asset = Optionals::OfNullable(Handle->GetLoadedAsset());
            } else {
                Asset = Optionals::OfNullable(Cast<T>(Handle->GetLoadedAsset()));
            }
            OnLoadCompleteDelegate.Broadcast(Asset);
        }
        
        TSharedPtr<FStreamableHandle> Handle;
        TOptional<AssetType> Asset;
        FOnAssetLoaded OnLoadCompleteDelegate;
    };
}
