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
    /**
     * Extract the value of a property from the given object.
     * <p>Note: This method is unsafe, use as your own risk.</p>
     * @tparam T The type of the property in question.
     * @param TargetObject The object to get the property of
     * @param PropertyName The name of the property to extract
     * @return The value of the property
     */
    template <typename T>
    static const T &GetPropertyValue(const UObject* TargetObject, FName PropertyName) {
        auto Property = TargetObject->GetClass()->FindPropertyByName(PropertyName);
        auto PropertyContainer = Property->ContainerPtrToValuePtr<void>(TargetObject);
        return TPropertyTypeFundamentals<T>::GetPropertyValue(PropertyContainer);
    }

    /**
     * Get all non-abstract subclasses of the given object type
     * @tparam T The object type to look up
     * @param TargetClass The class to check under
     * @return If list of found classes
     */
    template <typename T>
    requires std::is_base_of_v<UObject, T>
    static TArray<TSubclassOf<T>> GetAllSubclassesOfClass(TSubclassOf<T> TargetClass = T::StaticClass()) {
        TArray<TSubclassOf<T>> Subclasses;
        for (TObjectIterator<UClass> It; It; ++It) {
            if(It->IsChildOf(TargetClass) && !It->HasAnyClassFlags(CLASS_Abstract))
            {
                Subclasses.Add(*It);
            }
        }
        return Subclasses;
    }

};
