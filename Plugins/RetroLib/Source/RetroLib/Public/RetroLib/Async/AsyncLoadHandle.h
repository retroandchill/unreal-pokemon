// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifdef __UNREAL__
#if !RETROLIB_WITH_MODULES
#include <mutex>
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#endif

#include "RetroLib/Concepts/Delegates.h"
#include "RetroLib/Variants/VariantObject.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {
    template <typename T>
        requires std::is_base_of_v<UObject, T> || VariantObject<T>
    class TAsyncLoadHandle : public TSharedFromThis<TAsyncLoadHandle<T>> {
        struct FPrivateToken {
            explicit FPrivateToken() = default;
        };

      public:
        using ResultType = std::conditional_t<VariantObject<T>, T, T &>;
        DECLARE_MULTICAST_DELEGATE_OneParam(FAsyncLoadCompleteDelegate, const TOptional<ResultType> &);

        explicit TAsyncLoadHandle(FPrivateToken, const FSoftObjectPath &ObjectPath) : Handle(CreateHandle(ObjectPath)) {
        }

        static TSharedRef<TAsyncLoadHandle> Create(const FSoftObjectPath &ObjectPath) {
            auto Handle = MakeShared<TAsyncLoadHandle>(FPrivateToken(), ObjectPath);
            Handle->Handle->BindCompleteDelegate(FStreamableDelegate::CreateSP(Handle, &TAsyncLoadHandle::SetResult));
            Handle->Handle->StartStalledHandle();
            return Handle;
        }

        bool IsLoaded() const {
            return bLoadCompleted;
        }

        const TOptional<ResultType> &GetObject(float Timeout = 0.f) const {
            if (!IsLoaded()) {
                Handle->WaitUntilComplete(Timeout);
            }

            return Result;
        }

        template <typename F, typename... A>
            requires std::invocable<F, const TOptional<ResultType> &, A...> &&
                     Delegates::CanBindFree<typename FAsyncLoadCompleteDelegate::FDelegate, F, A...>
        void OnLoadComplete(F &&Functor, A &&...Args) {
            std::scoped_lock Lock(UpdateMutex);
            if (IsLoaded()) {
                std::invoke(std::forward<F>(Functor), Result, std::forward<A>(Args)...);
            } else {
                AddToDelegate(OnCompleteDelegate, std::forward<F>(Functor), std::forward<A>(Args)...);
            }
        }

        template <typename O, typename F, typename... A>
            requires std::invocable<F, O, const TOptional<ResultType> &, A...> &&
                     Delegates::CanBindMember<typename FAsyncLoadCompleteDelegate::FDelegate, O, F, A...>
        void OnLoadComplete(O &&Object, F &&Functor, A &&...Args) {
            std::scoped_lock Lock(UpdateMutex);
            if (IsLoaded()) {
                std::invoke(std::forward<F>(Functor), std::forward<O>(Object), Result, std::forward<A>(Args)...);
            } else {
                AddToDelegate(OnCompleteDelegate, std::forward<O>(Object), std::forward<F>(Functor),
                              std::forward<A>(Args)...);
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
#endif