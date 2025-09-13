// "Unreal Pokémon" created by Retro & Chill.

#include "RPGEntity.h"
#include "LogRPGCore.h"
#include "Misc/DataValidation.h"
#include "RPGComponent.h"

const UScriptStruct *URPGEntity::GetEntityStruct() const
{
    if (auto *EntityStruct = K2_GetEntityStruct(); EntityStruct != nullptr)
    {
        return EntityStruct;
    }

    return NativeGetEntityStruct();
}

URPGComponent *URPGEntity::GetComponent(const TSubclassOf<URPGComponent> ComponentClass) const
{
    for (auto Component : RequiredComponents)
    {
        if (Component->IsA(ComponentClass))
        {
            return Component;
        }
    }

    return nullptr;
}

void URPGEntity::PostInitProperties()
{
    UObject::PostInitProperties();

    GatherComponentReferences();
}

void URPGEntity::PostLoad()
{
    UObject::PostLoad();

    GatherComponentReferences();
}

#if WITH_EDITOR
EDataValidationResult URPGEntity::IsDataValid(FDataValidationContext &Context) const
{
    auto Result = UObject::IsDataValid(Context);

    // Validating this on abstract components just creates a bunch of noise
    if (GetClass()->HasAnyClassFlags(CLASS_Abstract))
        return Result;

    for (TFieldIterator<FProperty> PropIt(GetClass()); PropIt; ++PropIt)
    {
        const auto *ObjProp = CastField<FObjectProperty>(*PropIt);
        if (ObjProp == nullptr || !ObjProp->PropertyClass->IsChildOf(URPGComponent::StaticClass()))
            continue;

        if (const auto *Component = Cast<URPGComponent>(ObjProp->GetObjectPropertyValue_InContainer(this));
            Component == nullptr)
        {
            Context.AddError(
                FText::FormatOrdered(NSLOCTEXT("UnrealSharp", "ComponentNotFound", "Component {0} not found"),
                                     FText::FromString(ObjProp->GetName())));
            Result = EDataValidationResult::Invalid;
        }
    }

    return Result;
}

void URPGEntity::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
    UObject::PostEditChangeProperty(PropertyChangedEvent);

    GatherComponentReferences();
}
#endif

// ReSharper disable once CppMemberFunctionMayBeStatic
void URPGEntity::InitializeComponents(const int32 &){checkf(false, TEXT("This should never be called"))}

DEFINE_FUNCTION(URPGEntity::execInitializeComponents)
{
    auto *Self = CastChecked<URPGEntity>(Context);
    Stack.StepCompiledIn<FStructProperty>(nullptr);
    const auto ParamsProperty = CastFieldChecked<FStructProperty>(Stack.MostRecentProperty);
    auto *ParamsStruct = Stack.MostRecentPropertyAddress;

    P_NATIVE_BEGIN
    Self->InitializeComponents(FStructView(ParamsProperty->Struct, ParamsStruct));
    P_NATIVE_END
}

void URPGEntity::InitializeComponents(const FStructView Params)
{
    if (Params.GetScriptStruct() != GetEntityStruct())
    {
        UE_LOG(LogRPGCore, Error, TEXT("Invalid params struct for entity %s"), *GetName());
        return;
    }

    for (const auto Component : RequiredComponents)
    {
        Component->Initialize(Params);
    }
}

void URPGEntity::GatherComponentReferences()
{
    RequiredComponents.Reset();
    
    for (TFieldIterator<FProperty> PropIt(GetClass()); PropIt; ++PropIt)
    {
        const auto *ObjProp = CastField<FObjectProperty>(*PropIt);
        if (ObjProp == nullptr || !ObjProp->PropertyClass->IsChildOf(URPGComponent::StaticClass()))
            continue;

        if (auto *Component = Cast<URPGComponent>(ObjProp->GetObjectPropertyValue_InContainer(this));
            Component != nullptr)
        {
            RequiredComponents.Add(Component);
        }
    }
}