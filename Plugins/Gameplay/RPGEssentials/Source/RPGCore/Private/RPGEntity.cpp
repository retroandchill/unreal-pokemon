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
    for (auto Component : Components)
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
    DiscoverAndBindInitFunctions();
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
#endif

// ReSharper disable once CppMemberFunctionMayBeStatic
void URPGEntity::InitializeComponents(int32 &){checkf(false, TEXT("This should never be called"))}

DEFINE_FUNCTION(URPGEntity::execInitializeComponents)
{
    auto *Self = CastChecked<URPGEntity>(Context);
    Stack.StepCompiledIn<FStructProperty>(nullptr);
    const auto ParamsProperty = CastFieldChecked<FStructProperty>(Stack.MostRecentProperty);
    auto *ParamsStruct = Stack.MostRecentPropertyAddress;

    P_NATIVE_BEGIN
    if (ParamsProperty->Struct != Self->GetEntityStruct())
    {
        UE_LOG(LogRPGCore, Error, TEXT("Invalid params struct for entity %s"), *Self->GetName());
        return;
    }

    for (const auto Component : Self->Components)
    {
        if (Component->InitDelegate.IsBound())
        {
            Component->InitDelegate.Execute(FStructView(ParamsProperty->Struct, ParamsStruct));
        }
    }
    P_NATIVE_END
}

void URPGEntity::GatherComponentReferences()
{
    for (TFieldIterator<FProperty> PropIt(GetClass()); PropIt; ++PropIt)
    {
        const auto *ObjProp = CastField<FObjectProperty>(*PropIt);
        if (ObjProp == nullptr || !ObjProp->PropertyClass->IsChildOf(URPGComponent::StaticClass()))
            continue;

        if (auto *Component = Cast<URPGComponent>(ObjProp->GetObjectPropertyValue_InContainer(this));
            Component != nullptr)
        {
            Components.Add(Component);
        }
    }
}

void URPGEntity::DiscoverAndBindInitFunctions()
{
    auto *ExpectedStruct = GetEntityStruct();
    if (ExpectedStruct == nullptr)
    {
        return;
    }

    for (auto Component : Components)
    {
        DiscoverAndBindInitFunction(Component, ExpectedStruct);
    }
}

void URPGEntity::DiscoverAndBindInitFunction(URPGComponent *Component, const UScriptStruct *ExpectedStruct)
{
    static FName MetadataTag = RPGInitFunction;
    static TMap<TObjectPtr<const UScriptStruct>, TMap<TSubclassOf<URPGComponent>, TObjectPtr<UFunction>>> InitFunctions;

    auto &FunctionsMapping = InitFunctions.FindOrAdd(ExpectedStruct);
    if (const auto *Function = FunctionsMapping.Find(Component->GetClass()); Function != nullptr)
    {
        BindComponentFunction(Component, *Function);
        return;
    }

    for (TFieldIterator<UFunction> FuncIt(Component->GetClass()); FuncIt; ++FuncIt)
    {
        auto *Func = *FuncIt;
        if (Func->HasMetaData(MetadataTag))
            continue;

        if (Func->GetReturnProperty() != nullptr)
            continue;

        int32 ParamCount = 0;
        const FStructProperty *StructParam = nullptr;
        for (TFieldIterator<FProperty> ParamIt(Func); ParamIt; ++ParamIt)
        {
            if (ParamIt->HasAnyPropertyFlags(CPF_Parm))
            {
                ++ParamCount;
                if (const auto *AsStruct = CastField<FStructProperty>(*ParamIt); AsStruct != nullptr)
                    StructParam = AsStruct;
            }
        }

        if (ParamCount != 1 || StructParam == nullptr || StructParam->Struct != ExpectedStruct)
            continue;

        FunctionsMapping.Emplace(Component->GetClass(), Func);
        BindComponentFunction(Component, Func);
        return;
    }
}

void URPGEntity::BindComponentFunction(URPGComponent *Component, UFunction *Function)
{
    Component->InitDelegate.BindLambda(
        [Component, Function](const FStructView InitData) { Component->ProcessEvent(Function, InitData.GetMemory()); });
}