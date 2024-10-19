// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "VariantObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VariantObjectUtilities.generated.h"

struct FVariantObjectTemplate;

/**
 * Utility library for handling operations related to variant objects.
 */
UCLASS()
class UNREALRANGES_API UVariantObjectUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    /**
     * Create a variant struct object from the given object.
     * <p><b>NOTE:</b> This function should never be called from C++, and should only be called from Blueprints.</p>
     * @param Object The object in question
     * @param Variant The output struct.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, CustomThunk, Category = Variants,
        meta = (CustomStructureParam = "Variant"))
    static void CreateVariantFromObject(const UObject* Object, uint8& Variant);
	DECLARE_FUNCTION(execCreateVariantFromObject);

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, CustomThunk, Category = Variants,
        meta = (CustomStructureParam = "Variant"))
    static bool CreateVariantFromObjectChecked(TSubclassOf<UObject> Class, const UObject* Object, uint8& Variant);
    DECLARE_FUNCTION(execCreateVariantFromObjectChecked);

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, CustomThunk, Category = Variants,
        meta = (CustomStructureParam = "Variant"))
    static UObject* GetObjectFromVariant(const uint8& Variant);
    DECLARE_FUNCTION(execGetObjectFromVariant);

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, CustomThunk, Category = Variants,
        meta = (CustomStructureParam = "Variant",
            DeterminesOutputType = Class, DynamicOutputParam = Object))
    static bool GetObjectFromVariantChecked(TSubclassOf<UObject> Class, const uint8& Variant, UObject* &Object);
    DECLARE_FUNCTION(execGetObjectFromVariantChecked);

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, CustomThunk, Category = Variants,
        meta = (CustomStructureParam = "Variant,SoftVariant"))
    static void MakeSoftVariantFromVariant(const uint8& Variant, uint8& SoftVariant);
    DECLARE_FUNCTION(execMakeSoftVariantFromVariant);

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, CustomThunk, Category = Variants,
        meta = (CustomStructureParam = "SoftVariant"))
    static void MakeSoftVariantFromSoftObject(const TSoftObjectPtr<UObject>& Object, uint8& SoftVariant);
    DECLARE_FUNCTION(execMakeSoftVariantFromSoftObject);

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, CustomThunk, Category = Variants,
        meta = (CustomStructureParam = "SoftVariant"))
    static bool MakeSoftVariantFromSoftObjectChecked(const TSoftObjectPtr<UObject>& Object, uint8& SoftVariant);
    DECLARE_FUNCTION(execMakeSoftVariantFromSoftObjectChecked);

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, CustomThunk, Category = Variants,
        meta = (CustomStructureParam = "SoftVariant,Variant"))
    static bool LoadSynchronous(const uint8& SoftVariant, uint8& Variant);
    DECLARE_FUNCTION(execLoadSynchronous);
};
