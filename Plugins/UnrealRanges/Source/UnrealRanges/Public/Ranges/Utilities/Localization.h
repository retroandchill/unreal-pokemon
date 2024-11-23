// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 * Macro for handling creating localized text, using the filename for the namespace.
 *
 * @param Key The searchable text key
 * @param Text The text itself
 */
#define LOCALIZED_TEXT(Key, Text) NSLOCTEXT(__FILE__, Key, Text)