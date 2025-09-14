// "Unreal Pokémon" created by Retro & Chill.

#include "RPGEntity.h"
#include "LogRPGCore.h"
#include "Misc/DataValidation.h"
#include "RPGComponent.h"
#include "UnrealIteratorAdapter.h"

const UScriptStruct *URPGEntity::GetEntityStruct() const
{
    if (auto *EntityStruct = K2_GetEntityStruct(); EntityStruct != nullptr)
    {
        return EntityStruct;
    }

    return NativeGetEntityStruct();
}

void URPGEntity::PostInitProperties()
{
    UObject::PostInitProperties();

    if (HasAnyFlags(RF_ClassDefaultObject) || !HasAnyFlags(RF_NeedInitialization))
    {
        // Because of the way C# classes are loaded we need to defer this if we're still
        // loading the engine.
        if (GEngine != nullptr && GEngine->IsInitialized())
        {
            CreateRequiredComponents();
        }
        else
        {
            FCoreDelegates::OnPostEngineInit.AddWeakLambda(this, [this] {
                CreateRequiredComponents();
                GatherComponentReferences();
            });
        }
    }

    GatherComponentReferences();
}

void URPGEntity::PostLoad()
{
    UObject::PostLoad();

    GatherComponentReferences();
}

URPGComponent *URPGEntity::CreateComponent(const TSubclassOf<URPGComponent> ComponentClass)
{
    return NewObject<URPGComponent>(this, ComponentClass);
}

#if WITH_EDITOR
EDataValidationResult URPGEntity::IsDataValid(FDataValidationContext &Context) const
{
    auto Result = UObject::IsDataValid(Context);

    // Validating this on abstract components just creates a bunch of noise
    if (GetClass()->HasAnyClassFlags(CLASS_Abstract))
        return Result;

    for (auto *ObjProp : TFieldView<FObjectProperty>(GetClass(), EFieldIteratorFlags::IncludeSuper) |
                             ranges::views::filter([](const FObjectProperty *Prop) {
                                 return Prop->PropertyClass->IsChildOf(URPGComponent::StaticClass());
                             }))
    {
        if (const auto *Component = Cast<URPGComponent>(ObjProp->GetObjectPropertyValue_InContainer(this));
            Component == nullptr)
        {
            Context.AddError(
                FText::FormatOrdered(NSLOCTEXT("UnrealSharp", "ComponentNotFound", "Component {0} not found"),
                                     FText::FromString(ObjProp->GetName())));
            Result = EDataValidationResult::Invalid;
        }
    }

    for (auto [Index, Component] : AdditionalComponents | ranges::views::enumerate)
    {
        if (Component == nullptr)
        {
            Context.AddError(
                FText::FormatOrdered(NSLOCTEXT("UnrealSharp", "ComponentNotFound", "Additional component at index {0}"),
                                     FText::FromString(FString::FromInt(Index))));
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

void URPGEntity::InitializeComponents()
{
    InitializeComponents(FStructView());
}

// ReSharper disable once CppMemberFunctionMayBeConst
void URPGEntity::InitializeComponents(const FStructView Params)
{
    for (URPGComponent *Component : GetAllComponents())
    {
        Component->PreInitialize(this);
    }
    
    auto *EntityStruct = GetEntityStruct();
    bool bCallParameterizedInit = EntityStruct != nullptr;
    if (bCallParameterizedInit && EntityStruct != Params.GetScriptStruct())
    {
        UE_LOG(LogRPGCore, Error, TEXT("Invalid params struct for entity %s"), *GetName());
        bCallParameterizedInit = false;
    }

    for (URPGComponent *Component : GetAllComponents())
    {
        if (bCallParameterizedInit)
        {
            Component->Initialize(Params);
        }
    }
}

URPGComponent *URPGEntity::GetComponentInternal(const TSubclassOf<URPGComponent> ComponentClass) const
{
    for (URPGComponent *Component : GetAllComponents())
    {
        if (Component->IsA(ComponentClass))
        {
            return Component;
        }
    }

    return nullptr;
}

void URPGEntity::DefaultInitializeComponents()
{
    InitializeComponents();
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