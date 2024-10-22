// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

#define NUMBER_LITERAL(Number) #Number

#define TEXT_LITERAL(Text) NSLOCTEXT(__FILE__, __FUNCTION__ NUMBER_LITERAL(__LINE__), Text)