// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "RetroLib/Functional/Delegates.h"
#include "RetroLib/RetroLibMacros.h"
#include "Simple2D/Proxies/FlipbookProxy.h"

namespace Simple2D {

    /**
     *
     */
    class SIMPLE2D_API FFlipbookTicker {
        DECLARE_MULTICAST_DELEGATE_OneParam(FOnFrameIndexChanged, std::any)

            public : FFlipbookTicker() = default;

        template <Flipbook T>
        explicit FFlipbookTicker(T *Object) : Proxy(Object) {
        }

        void TickFlipbook(float DeltaTime);

        FFlipbookProxy &GetFlipbook() {
            return Proxy;
        }

        template <Flipbook T>
        bool SetFlipbook(T *NewFlipbook) {
            if (NewFlipbook != Proxy) {
                Proxy = NewFlipbook;

                // We need to also reset the frame and time also
                AccumulatedTime = 0.0f;
                CalculateCurrentFrame();

                return true;
            }

            return false;
        }

        void Play();

        void PlayFromStart();

        void Reverse();

        void ReverseFromEnd();

        void Stop();

        bool IsPlaying() const;

        bool IsReversing() const;

        void SetPlaybackPositionInFrames(int32 NewFramePosition, bool bFireEvents);

        int32 GetPlaybackPositionInFrames() const;

        void SetPlaybackPosition(float NewPosition);

        float GetPlaybackPosition() const;

        void SetLooping(bool bNewLooping);

        bool IsLooping() const;

        void SetPlayRate(float NewRate);

        float GetPlayRate() const;

        void SetNewTime(float NewTime);

        float GetFlipbookLength() const;

        int32 GetFlipbookLengthInFrames() const;

        float GetFlipbookFramerate() const;

        void CalculateCurrentFrame();

        RETRO_MULTICAST_DELEGATE_MEMBER(FOnFrameIndexChanged, OnFrameIndexChanged)
        RETRO_MULTICAST_DELEGATE_MEMBER(FSimpleMulticastDelegate, OnFinishedPlaying)

      private:
        FFlipbookProxy Proxy;

        float PlayRate = 1.0;
        uint8 bLooping : 1 = true;
        uint8 bReversePlayback : 1 = false;
        uint8 bPlaying : 1 = true;

        float AccumulatedTime = 0.f;

        int32 CachedFrameIndex = INDEX_NONE;
    };
} // namespace Simple2D