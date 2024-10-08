// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Ranges/Variants/VariantObjectStruct.h"
#include <mutex>

namespace UE::Ranges {
    /**
     * 
     */
    template <typename T>
        requires std::is_base_of_v<UObject, T> || VariantObject<T>
    class TAsyncLoadHandle : public TSharedFromThis<TAsyncLoadHandle<T>> {
        struct FPrivateToken {
            explicit FPrivateToken() = default;
        };
        
    public:
        using ResultType = std::conditional_t<VariantObject<T>, T, T&>;
        DECLARE_MULTICAST_DELEGATE_OneParam(FAsyncLoadCompleteDelegate, const TOptional<ResultType>&);
        
        explicit TAsyncLoadHandle(FPrivateToken, const FSoftObjectPath &ObjectPath) : Handle(CreateHandle(ObjectPath)) {
        }

        static TSharedRef<TAsyncLoadHandle> Create(const FSoftObjectPath &ObjectPath) {
            auto Handle = MakeShared<TAsyncLoadHandle>(FPrivateToken(), ObjectPath);
            Handle->Handle->BindCompleteDelegate(FStreamableDelegate::CreateSP(Handle, &TAsyncLoadHandle::SetResult));
            Handle->Handle->StartStreaming();
            return Handle;
        }

        bool IsLoaded() const {
            return bLoadCompleted;
        }

        const TOptional<ResultType>& GetObject(float Timeout = 0.f) const {
            if (!IsLoaded()) {
                Handle->WaitUntilComplete(Timeout);
            }

            return Result;
        }

        template <typename F, typename... A>
            requires std::invocable<F, const TOptional<ResultType>&, A...> &&
                CanBindDelegate<FAsyncLoadCompleteDelegate, F, A...>
        void OnLoadComplete(F&& Functor, A&&... Args) {
            std::scoped_lock Lock(UpdateMutex);
            if (IsLoaded()) {
                std::invoke(Forward<F>(Functor), Result, Forward<A>(Args)...);
            } else {
                Handle->BindCompleteDelegate(CreateDelegate<FAsyncLoadCompleteDelegate, F, A...>(Forward<F>(Functor), Forward<A>(Args)...));
            }
        }

        template <typename O, typename F, typename... A>
            requires std::invocable<F, O, const TOptional<ResultType>&, A...> &&
                CanBindDelegate<FAsyncLoadCompleteDelegate, O, F, A...>
        void OnLoadComplete(O&& Object, F&& Functor, A&&... Args) {
            std::scoped_lock Lock(UpdateMutex);
            if (IsLoaded()) {
                std::invoke(Forward<F>(Functor), Forward<O>(Object), Result, Forward<A>(Args)...);
            } else {
                Handle->BindCompleteDelegate(CreateDelegate<FAsyncLoadCompleteDelegate, O, F, A...>(Forward<O>(Object), Forward<F>(Functor), Forward<A>(Args)...));
            }
        }

    private:
        static TSharedRef<FStreamableHandle> CreateHandle(const FSoftObjectPath &ObjectPath) {
            auto &StreamableManager = UAssetManager::GetStreamableManager();
            auto Handle = StreamableManager.RequestAsyncLoad(ObjectPath, FStreamableDelegate(),
                FStreamableManager::DefaultAsyncLoadPriority, false, true);
            return Handle.ToSharedRef();
        }

        void SetResult() {
            std::scoped_lock Lock(UpdateMutex);
            if constexpr (VariantObject<T>) {
                if (auto Object = Handle->GetLoadedAsset(); T::IsValidType(Object)) {
                    Result.Emplace(Object);
                }
            } else {
                Result = Optionals::OfNullable(Cast<T>(Handle->GetLoadedAsset()));
            }
            OnCompleteDelegate.Broadcast(Result);
            bLoadCompleted = true;
        }

        TSharedRef<FStreamableHandle> Handle;
        TOptional<ResultType> Result;
        FAsyncLoadCompleteDelegate OnCompleteDelegate;
        bool bLoadCompleted = false;
        std::mutex UpdateMutex;
    };
}