// "Unreal Pokémon" created by Retro & Chill.


#include "Simple2D/Components/FlipbookTicker.h"


namespace Simple2D {
    void FFlipbookTicker::TickFlipbook(float DeltaTime) {
        bool bIsFinished = false;

        if (bPlaying)
        {
            const float TimelineLength = GetFlipbookLength();
            const float EffectiveDeltaTime = DeltaTime * (bReversePlayback ? (-PlayRate) : (PlayRate));

            float NewPosition = AccumulatedTime + EffectiveDeltaTime;

            if (EffectiveDeltaTime > 0.0f)
            {
                if (NewPosition > TimelineLength)
                {
                    if (bLooping)
                    {
                        // If looping, play to end, jump to start, and set target to somewhere near the beginning.
                        SetPlaybackPosition(TimelineLength, true);
                        SetPlaybackPosition(0.0f, false);

                        if (TimelineLength > 0.0f)
                        {
                            while (NewPosition > TimelineLength)
                            {
                                NewPosition -= TimelineLength;
                            }
                        }
                        else
                        {
                            NewPosition = 0.0f;
                        }
                    }
                    else
                    {
                        // If not looping, snap to end and stop playing.
                        NewPosition = TimelineLength;
                        Stop();
                        bIsFinished = true;
                    }
                }
            }
            else
            {
                if (NewPosition < 0.0f)
                {
                    if (bLooping)
                    {
                        // If looping, play to start, jump to end, and set target to somewhere near the end.
                        SetPlaybackPosition(0.0f, true);
                        SetPlaybackPosition(TimelineLength, false);

                        if (TimelineLength > 0.0f)
                        {
                            while (NewPosition < 0.0f)
                            {
                                NewPosition += TimelineLength;
                            }
                        }
                        else
                        {
                            NewPosition = 0.0f;
                        }
                    }
                    else
                    {
                        // If not looping, snap to start and stop playing.
                        NewPosition = 0.0f;
                        Stop();
                        bIsFinished = true;
                    }
                }
            }

            SetPlaybackPosition(NewPosition, true);
        }

        // Notify user that the flipbook finished playing
        if (bIsFinished)
        {
            OnFinishedPlaying.Broadcast();
        }
    }

    void FFlipbookTicker::Play() {
        bReversePlayback = false;
        bPlaying = true;
    }

    void FFlipbookTicker::PlayFromStart() {
        SetPlaybackPosition(0.0f, false);
        Play();
    }

    void FFlipbookTicker::Reverse() {
        bReversePlayback = true;
        bPlaying = true;
    }

    void FFlipbookTicker::ReverseFromEnd() {
        SetPlaybackPosition(GetFlipbookLength(), false);
        Reverse();
    }

    void FFlipbookTicker::Stop() {
        bPlaying = false;
    }

    bool FFlipbookTicker::IsPlaying() const {
        return bPlaying;
    }

    bool FFlipbookTicker::IsReversing() const {
        return bPlaying && bReversePlayback;
    }

    void FFlipbookTicker::SetPlaybackPositionInFrames(int32 NewFramePosition, bool bFireEvents) {
        const float Framerate = GetFlipbookFramerate();
        const float NewTime = Framerate > 0.0f ? (NewFramePosition / Framerate) : 0.0f;
        SetPlaybackPosition(NewTime, bFireEvents);
    }

    int32 FFlipbookTicker::GetPlaybackPositionInFrames() const {
        const float Framerate = GetFlipbookFramerate();
        if (const int32 NumFrames = GetFlipbookLengthInFrames(); NumFrames > 0)
        {
            return FMath::Clamp<int32>(FMath::TruncToInt(AccumulatedTime * Framerate), 0, NumFrames - 1);
        }
        
        return 0;
    }

    void FFlipbookTicker::SetPlaybackPosition(float NewPosition, bool bFireEvents) {
        float OldPosition = AccumulatedTime;
        AccumulatedTime = NewPosition;

        // If we should be firing events for this track...
        if (bFireEvents)
        {
            float MinTime;
            float MaxTime;
            if (!bReversePlayback)
            {
                // If playing sequence forwards.
                MinTime = OldPosition;
                MaxTime = AccumulatedTime;

                // Slight hack here.. if playing forwards and reaching the end of the sequence, force it over a little to ensure we fire events actually on the end of the sequence.
                if (MaxTime == GetFlipbookLength())
                {
                    MaxTime += (float)KINDA_SMALL_NUMBER;
                }
            }
            else
            {
                // If playing sequence backwards.
                MinTime = AccumulatedTime;
                MaxTime = OldPosition;

                // Same small hack as above for backwards case.
                if (MinTime == 0.0f)
                {
                    MinTime -= (float)KINDA_SMALL_NUMBER;
                }
            }
        }

        if (OldPosition != AccumulatedTime)
        {
            CalculateCurrentFrame();
        }
    }

    float FFlipbookTicker::GetPlaybackPosition() const {
	    return AccumulatedTime;
    }

    void FFlipbookTicker::SetLooping(bool bNewLooping) {
        bLooping = bNewLooping;
    }

    bool FFlipbookTicker::IsLooping() const {
        return bLooping;
    }

    void FFlipbookTicker::SetPlayRate(float NewRate) {
        PlayRate = NewRate;
    }

    float FFlipbookTicker::GetPlayRate() const {
        return PlayRate;
    }

    void FFlipbookTicker::SetNewTime(float NewTime) {
        SetPlaybackPosition(NewTime, false);
    }

    float FFlipbookTicker::GetFlipbookLength() const {
        return Proxy != nullptr ? Proxy.GetTotalDuration() : 0.0f;
    }

    int32 FFlipbookTicker::GetFlipbookLengthInFrames() const {
        return Proxy != nullptr ? Proxy.GetNumFrames() : 0;
    }

    float FFlipbookTicker::GetFlipbookFramerate() const {
        return Proxy != nullptr ? Proxy.GetFramesPerSecond() : 15.0f;
    }

    void FFlipbookTicker::CalculateCurrentFrame() {
        const int32 LastCachedFrame = CachedFrameIndex;
        CachedFrameIndex = (Proxy != nullptr) ? Proxy.GetKeyFrameIndexAtTime(AccumulatedTime) : INDEX_NONE;

        if (CachedFrameIndex != LastCachedFrame)
        {
            OnFrameIndexChanged.Broadcast(Proxy.GetKeyFrameChecked(CachedFrameIndex));
        }
    }
}