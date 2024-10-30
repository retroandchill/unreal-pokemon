// "Unreal Pokémon" created by Retro & Chill.


#include "Ranges/Variants/OpaqueEnum.h"

UE::Ranges::FOpaqueEnum::FOpaqueEnum(UEnum *Enum, int64 Value) : NumericValue(Value), Enum(Enum) {
}