// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "RetroLib/Concepts/Structs.h"
#include <any>

namespace Simple2D {
    template <typename T>
    concept FlipbookKeyFrame = Retro::UEStruct<T> && requires(T&& KeyFrame) {
        { std::forward<T>(KeyFrame).FrameRun } -> std::convertible_to<int32>;
    };
    
    template <typename T>
    concept Flipbook = std::derived_from<T, UObject> && requires(const T* Object, int32 Index, float Time, bool bClampToEnds) {
        { Object->GetNumFrames() } -> std::convertible_to<int32>;
        { Object->GetFramesPerSecond() } -> std::convertible_to<float>;
        { Object->GetNumKeyFrames() } -> std::convertible_to<int32>;
        { Object->GetTotalDuration() } -> std::convertible_to<float>;
        { Object->GetRenderBounds() } -> std::convertible_to<FBoxSphereBounds>;
        { Object->GetKeyFrameIndexAtTime(Time, bClampToEnds) } -> std::convertible_to<int32>;
        { Object->GetKeyFrameChecked(Index) } -> FlipbookKeyFrame;
        { Object->IsValidKeyFrameIndex(Index) } -> std::same_as<bool>;
        { Object->GetDefaultMaterial() } -> std::convertible_to<UMaterialInterface*>;
    } && requires(T* Object) {
        Object->InvalidateCachedData();
    };
    
    class FFlipbookProxy : public FGCObject {
    public:
        FFlipbookProxy() = default;
        
        template <Flipbook T>
        explicit FFlipbookProxy(T* Object) : ProxiedFlipbook(Object), VTable(&GetVTable<T>()) {}

        template <Flipbook T>
        FFlipbookProxy& operator=(T* Object) {
            ProxiedFlipbook = Object;
            VTable = &GetVTable<T>();
            return *this;
        }
        
        void AddReferencedObjects(FReferenceCollector &Collector) final {
            Collector.AddReferencedObject(ProxiedFlipbook);
        }
        
        FString GetReferencerName() const final {
            return TEXT("FFlipbookProxy");
        }
        
        int32 GetNumFrames() const {
            return VTable->GetNumFrames(ProxiedFlipbook);
        }
        
        float GetFramesPerSecond() const {
            return VTable->GetFramesPerSecond(ProxiedFlipbook);
        }
        
        int32 GetNumKeyFrames() const {
            return VTable->GetNumKeyFrames(ProxiedFlipbook);
        }
        
        float GetTotalDuration() const {
            return VTable->GetTotalDuration(ProxiedFlipbook);
        }
        
        int32 GetKeyFrameIndexAtTime(float Time, bool bClampToEnds = false) const {
            return VTable->GetKeyFrameIndexAtTime(ProxiedFlipbook, Time, bClampToEnds);
        }

        std::any GetKeyFrameChecked(int32 Index) {
            return VTable->GetKeyFrameAtIndex(ProxiedFlipbook, Index);
        }
        
        int32 GetFrameRunAtIndex(int32 Index) const {
            return VTable->GetFrameRunAtIndex(ProxiedFlipbook, Index);
        }
        
        bool IsValidKeyFrameIndex(int32 Index) const {
            return VTable->IsValidKeyFrameIndex(ProxiedFlipbook, Index);
        }
        
        FBoxSphereBounds GetRenderBounds() const {
            return VTable->GetRenderBounds(ProxiedFlipbook);
        }
        
        UMaterialInterface * GetDefaultMaterial() const {
            return VTable->GetDefaultMaterial(ProxiedFlipbook);
        }
        
        void InvalidateCachedData() {
            VTable->InvalidateCachedData(ProxiedFlipbook);
        }

        template <Flipbook T>
        friend bool operator==(const FFlipbookProxy& A, const T* B) {
            return A.ProxiedFlipbook == B;
        }

        template <Flipbook T>
        friend bool operator==(const T* A, const FFlipbookProxy& B) {
            return A == B.ProxiedFlipbook;
        }
        
        friend bool operator==(const FFlipbookProxy& A, std::nullptr_t) {
            return A.ProxiedFlipbook == nullptr;
        }
        
        friend bool operator==(std::nullptr_t, const FFlipbookProxy& B) {
            return nullptr == B.ProxiedFlipbook;
        }

    private:
        struct FVTable {
            int32(*GetNumFrames)(const UObject* Object);
            float(*GetFramesPerSecond)(const UObject* Object);
            int32(*GetNumKeyFrames)(const UObject* Object);
            float(*GetTotalDuration)(const UObject* Object);
            int32 (*GetKeyFrameIndexAtTime)(const UObject* Object, float Time, bool bClampToEnds);
            std::any(*GetKeyFrameAtIndex)(const UObject* Object, int32 Index);
            int32(*GetFrameRunAtIndex)(const UObject* Object, int32 Index);
            bool(*IsValidKeyFrameIndex)(const UObject* Object, int32 Index);
            FBoxSphereBounds(*GetRenderBounds)(const UObject* Object);
            UMaterialInterface*(*GetDefaultMaterial)(const UObject* Object);
            void(*InvalidateCachedData)(UObject* Object);
        };

        template <Flipbook T>
        struct TVTableImpl {
            static int32 GetNumFrames(const UObject *Object) {
                return static_cast<const T*>(Object)->GetNumFrames();
            }

            static float GetFramesPerSecond(const UObject *Object) {
                return static_cast<const T*>(Object)->GetFramesPerSecond();
            }

            static int32 GetNumKeyFrames(const UObject *Object) {
                return static_cast<const T*>(Object)->GetNumKeyFrames();
            }

            static float GetTotalDuration(const UObject *Object) {
                return static_cast<const T*>(Object)->GetTotalDuration();
            }

            static int32 GetKeyFrameIndexAtTime(const UObject *Object, float Time, bool bClampToEnds) {
                return static_cast<const T*>(Object)->GetKeyFrameIndexAtTime(Time, bClampToEnds);
            }

            static std::any GetKeyFrameAtIndex(const UObject *Object, int32 Index) {
                return static_cast<const T*>(Object)->GetKeyFrameChecked(Index);
            }

            static int32 GetFrameRunAtIndex(const UObject *Object, int32 Index) {
                return static_cast<const T*>(Object)->GetKeyFrameChecked(Index).FrameRun;
            }

            static bool IsValidKeyFrameIndex(const UObject *Object, int32 Index) {
                return static_cast<const T*>(Object)->IsValidKeyFrameIndex(Index);
            }

            static FBoxSphereBounds GetRenderBounds(const UObject *Object) {
                return static_cast<const T*>(Object)->GetRenderBounds();
            }

            static UMaterialInterface *GetDefaultMaterial(const UObject *Object) {
                return static_cast<const T*>(Object)->GetDefaultMaterial();
            }

            static void InvalidateCachedData(UObject *Object) {
                static_cast<T*>(Object)->InvalidateCachedData();
            }
        };

        template <Flipbook T>
        static FVTable& GetVTable() {
            static FVTable VTable = {
                .GetNumFrames = TVTableImpl<T>::GetNumFrames,
                .GetFramesPerSecond = TVTableImpl<T>::GetFramesPerSecond,
                .GetNumKeyFrames = TVTableImpl<T>::GetNumKeyFrames,
                .GetTotalDuration = TVTableImpl<T>::GetTotalDuration,
                .GetKeyFrameIndexAtTime = TVTableImpl<T>::GetKeyFrameIndexAtTime,
                .GetKeyFrameAtIndex = TVTableImpl<T>::GetKeyFrameAtIndex,
                .GetFrameRunAtIndex = TVTableImpl<T>::GetFrameRunAtIndex,
                .IsValidKeyFrameIndex = TVTableImpl<T>::IsValidKeyFrameIndex,
                .GetRenderBounds = TVTableImpl<T>::GetRenderBounds,
                .GetDefaultMaterial = TVTableImpl<T>::GetDefaultMaterial,
                .InvalidateCachedData = TVTableImpl<T>::InvalidateCachedData
            };

            return VTable;
        }
        
        TObjectPtr<UObject> ProxiedFlipbook;
        FVTable* VTable = nullptr;
    };

    
    
}