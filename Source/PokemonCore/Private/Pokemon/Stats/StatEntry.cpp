// "Unreal Pokémon" created by Retro & Chill.
#include "Pokemon/Stats/StatEntry.h"

IMPLEMENT_ABSTRACT_METATYPE(IStatEntry)

bool IStatEntry::operator==(const IStatEntry& Other) const {
	return Equals(Other);
}
