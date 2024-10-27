#include "Ranges/Utilities/Casts.h"

bool UE::Ranges::TypesMatch(const UObject &Object, const UClass &Class) {
    if (Class.HasAnyClassFlags(CLASS_Interface)) {
        return Object.GetClass()->ImplementsInterface(&Class);
    }

    return Object.IsA(&Class);
}