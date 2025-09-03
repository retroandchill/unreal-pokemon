// Copyright © Laura Andelare
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted (subject to the limitations in the disclaimer
// below) provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
// THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
// CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
// NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "UE5Coro/LatentAwaiter.h"
#include "Engine/World.h"
#include "UE5Coro/CoroutineAwaiter.h"

using namespace UE5Coro;
using namespace UE5Coro::Private;

namespace
{
bool WaitUntilFrame(void* State, bool)
{
	return GFrameCounter >= reinterpret_cast<uint64>(State);
}

template<auto GetTime>
bool WaitUntilTime(void* State, bool bCleanup)
{
	// Don't attempt to access GWorld in this case, it could be nullptr
	if (bCleanup) [[unlikely]]
		return false;

	auto& TargetTime = reinterpret_cast<double&>(State);
	checkf(IsValid(GWorld),
	       TEXT("Internal error: latent poll outside of a valid world"));
	return (GWorld->*GetTime)() >= TargetTime;
}

bool WaitUntilPredicate(void* State, bool bCleanup)
{
	auto* Function = static_cast<std::function<bool()>*>(State);
	if (bCleanup) [[unlikely]]
	{
		delete Function;
		return false;
	}

	return (*Function)();
}

template<auto GetTime, bool bTimeIsOffset>
FLatentAwaiter GenericUntil(double Time)
{
#if ENABLE_NAN_DIAGNOSTIC
	if (FMath::IsNaN(Time))
	{
		logOrEnsureNanError(TEXT("Latent wait started with NaN time"));
	}
#endif
	checkf(IsInGameThread(),
	       TEXT("Latent awaiters may only be used on the game thread"));
	checkf(IsValid(GWorld),
	       TEXT("This function may only be used in the context of a valid world"));

	if constexpr (bTimeIsOffset)
		Time += (GWorld->*GetTime)();

	ensureMsgf((GWorld->*GetTime)() <= Time,
	           TEXT("Latent wait will finish immediately"));

	// Definition.h validates that a double fits into a void*
	void* State = nullptr;
	reinterpret_cast<double&>(State) = Time;
	return FLatentAwaiter(State, &WaitUntilTime<GetTime>, std::true_type());
}

struct FCustomTimeDilationAwaiterState
{
	TWeakObjectPtr<AActor> Actor;
	double Remaining;
	double PreviousTime;
};
}

template<auto GetTime>
struct FCustomTimeDilationAwaiter::TState : FCustomTimeDilationAwaiterState
{
	explicit TState(AActor* InActor, double InRemaining)
	{
#if ENABLE_NAN_DIAGNOSTIC
		if (FMath::IsNaN(Remaining))
		{
			logOrEnsureNanError(TEXT("Latent wait started with NaN time"));
		}
#endif
		checkf(IsInGameThread(),
		       TEXT("Latent awaiters may only be used on the game thread"));
		checkf(::IsValid(GWorld),
		       TEXT("This awaiter may only be used in the context of a valid world"));
		Actor = InActor;
		Remaining = InRemaining;
		PreviousTime = (GWorld->*GetTime)();
	}

	static bool ShouldResume(void* State, bool bCleanup)
	{
		auto* This = static_cast<TState*>(State);
		if (bCleanup) [[unlikely]]
		{
			delete This;
			return false;
		}

		auto* Actor = This->Actor.Get();
		if (!Actor)
			return true;

		checkf(::IsValid(GWorld),
		       TEXT("Internal error: latent poll outside of a valid world"));
		auto Time = (GWorld->*GetTime)();
		auto DeltaSeconds = Time - std::exchange(This->PreviousTime, Time);
		DeltaSeconds *= Actor->CustomTimeDilation;
		This->Remaining -= DeltaSeconds;
		return This->Remaining <= 0;
	}
};

template<auto GetTime>
FCustomTimeDilationAwaiter::FCustomTimeDilationAwaiter(TState<GetTime>* State)
	: FLatentAwaiter(State, &TState<GetTime>::ShouldResume, std::true_type())
{
}

bool FCustomTimeDilationAwaiter::await_resume()
{
	checkf(IsInGameThread(),
	       TEXT("Internal error: expected resumption on the game thread"));
	return static_cast<FCustomTimeDilationAwaiterState*>(State)->Actor.IsValid();
}

FLatentAwaiter Latent::NextTick()
{
	return Ticks(1);
}

FLatentAwaiter Latent::Ticks(int64 Ticks)
{
	ensureMsgf(Ticks >= 0, TEXT("Invalid number of ticks %lld"), Ticks);
	uint64 Target = GFrameCounter + Ticks;
	return FLatentAwaiter(reinterpret_cast<void*>(Target), &WaitUntilFrame,
	                      std::false_type());
}

FLatentAwaiter Latent::Until(std::function<bool()> Function)
{
	checkf(Function, TEXT("Provided function is empty"));
	return FLatentAwaiter(new std::function(std::move(Function)),
	                      &WaitUntilPredicate, std::false_type());
}

FLatentAwaiter Latent::Seconds(double Seconds)
{
	return GenericUntil<&UWorld::GetTimeSeconds, true>(Seconds);
}

FCustomTimeDilationAwaiter Latent::SecondsForActor(AActor* Actor, double Seconds)
{
	return FCustomTimeDilationAwaiter(
		new FCustomTimeDilationAwaiter::TState<&UWorld::GetTimeSeconds>(
			Actor, Seconds));
}

FCustomTimeDilationAwaiter Latent::UnpausedSecondsForActor(AActor* Actor,
                                                           double Seconds)
{
	return FCustomTimeDilationAwaiter(
		new FCustomTimeDilationAwaiter::TState<&UWorld::GetUnpausedTimeSeconds>(
			Actor, Seconds));
}

FLatentAwaiter Latent::UnpausedSeconds(double Seconds)
{
	return GenericUntil<&UWorld::GetUnpausedTimeSeconds, true>(Seconds);
}

FLatentAwaiter Latent::RealSeconds(double Seconds)
{
	return GenericUntil<&UWorld::GetRealTimeSeconds, true>(Seconds);
}

FLatentAwaiter Latent::AudioSeconds(double Seconds)
{
	return GenericUntil<&UWorld::GetAudioTimeSeconds, true>(Seconds);
}

FLatentAwaiter Latent::UntilTime(double Seconds)
{
	return GenericUntil<&UWorld::GetTimeSeconds, false>(Seconds);
}

FLatentAwaiter Latent::UntilUnpausedTime(double Seconds)
{
	return GenericUntil<&UWorld::GetUnpausedTimeSeconds, false>(Seconds);
}

FLatentAwaiter Latent::UntilRealTime(double Seconds)
{
	return GenericUntil<&UWorld::GetRealTimeSeconds, false>(Seconds);
}

FLatentAwaiter Latent::UntilAudioTime(double Seconds)
{
	return GenericUntil<&UWorld::GetAudioTimeSeconds, false>(Seconds);
}
