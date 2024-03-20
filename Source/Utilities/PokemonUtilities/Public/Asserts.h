// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#ifdef UE_EDITOR
#define ASSERT(Condition) if (!(Condition)) UE_LOG(LogBlueprint, Fatal, TEXT("Statement (%s) Evaluated to false!"), TEXT(#Condition))
#else
#define ASSERT(Condition)
#endif

#define GUARD(Condition, ReturnValue) \
if (Condition) { \
return ReturnValue; \
}

#ifdef UE_EDITOR
#define GUARD_WARN(Condition, ReturnValue, ...) \
 if (Condition) { \
  UE_LOG(LogBlueprint, Warning, __VA_ARGS__) \
  return ReturnValue; \
  }
#else
#define GUARD_WARN(Condition, ReturnValue, ...) \
 if (Condition) { \
  return ReturnValue; \
 }
#endif