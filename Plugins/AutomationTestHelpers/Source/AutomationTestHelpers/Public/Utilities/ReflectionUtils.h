// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ReflectionUtils.generated.h"

/**
 * Utility library used to aid in getting reflection properties by name.
 */
UCLASS()
class AUTOMATIONTESTHELPERS_API UReflectionUtils : public UBlueprintFunctionLibrary {
    GENERATED_BODY()
    
public:
    template <typename T>
    static const T &GetPropertyValue(const UObject* TargetObject, FName PropertyName) {
        auto Property = TargetObject->GetClass()->FindPropertyByName(PropertyName);
        auto PropertyContainer = Property->ContainerPtrToValuePtr<void>(TargetObject);
        return TPropertyTypeFundamentals<T>::GetPropertyValue(PropertyContainer);
    }

};
