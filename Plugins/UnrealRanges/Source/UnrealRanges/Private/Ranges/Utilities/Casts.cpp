#include "Ranges/Utilities/Casts.h"

bool UE::Ranges::TypesMatch(const UObject &Object, const UClass &Class) {
    if (Class.HasAnyClassFlags(CLASS_Interface)) {
        return Object.GetClass()->ImplementsInterface(&Class);
    }

    return Object.IsA(&Class);
}

bool UE::Ranges::IsInstantiableClass(const UClass *Class) {
    auto ClassName = Class->GetName();
    ClassName.ToUpperInline();
    // We don't want skeleton or reinstantiated classes to be loaded.
    if (ClassName.StartsWith("SKEL_") || ClassName.StartsWith("REINST_")) {
        return false;
    }

    return true;
}