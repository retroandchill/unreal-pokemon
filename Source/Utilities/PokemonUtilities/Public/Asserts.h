// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#ifdef UE_EDITOR
#define ASSERT(Condition) if (!(Condition)) UE_LOG(LogBlueprint, Fatal, TEXT("Statement (%s) Evaluated to false!"), TEXT(#Condition))
#else
#define ASSERT(Condition)
#endif