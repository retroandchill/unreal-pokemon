#pragma once

#ifdef __UNREAL__
#include "RetroLib/Async/GameThreadFutureExecutor.h"

#if RETROLIB_WITH_UE5CORO
#include "RetroLib/Async/BlueprintCoroutineActionBase.h"
#endif
#endif