// "Unreal Pokémon" created by Retro & Chill.

#include "Details/RPGComponentInitializerCustomization.h"
#include "DetailWidgetRow.h"
#include "RPGComponent.h"
#include "RPGEntity.h"
#include "UnrealIteratorAdapter.h"

TSharedRef<IPropertyTypeCustomization> FRPGComponentInitializerCustomization::MakeInstance()
{
    return MakeShared<FRPGComponentInitializerCustomization>();
}

void FRPGComponentInitializerCustomization::CustomizeHeader(const TSharedRef<IPropertyHandle> PropertyHandle,
                                                            FDetailWidgetRow &HeaderRow,
                                                            IPropertyTypeCustomizationUtils &CustomizationUtils)
{
    OuterPropertyHandle = PropertyHandle;

    // clang-format off
    HeaderRow.NameContent()
        [
            PropertyHandle->CreatePropertyNameWidget()
        ]
        .ValueContent()
        [
            SNew(SComboButton)
                .OnGetMenuContent(this, &FRPGComponentInitializerCustomization::MakeFunctionPicker)
                .ContentPadding(2)
                .ButtonContent()
                [
                    SNew(STextBlock)
                    .Text(this, &FRPGComponentInitializerCustomization::GetCurrentFunctionText)
                ]
        ];
    // clang-format on
}

void FRPGComponentInitializerCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
                                                              IDetailChildrenBuilder &ChildBuilder,
                                                              IPropertyTypeCustomizationUtils &CustomizationUtils)
{
    // No child customization
}

TSharedRef<SWidget> FRPGComponentInitializerCustomization::MakeFunctionPicker() const
{
    FMenuBuilder MenuBuilder(true, nullptr);

    // Get owning component
    TArray<UObject *> Outers;
    OuterPropertyHandle->GetOuterObjects(Outers);
    auto *Outer = Outers.Num() == 1 ? Outers[0] : nullptr;
    const auto *Comp = Cast<URPGComponent>(Outer);
    if (Comp == nullptr)
        return MenuBuilder.MakeWidget();

    // Get expected struct from owning entity
    const auto *Owner = Comp->GetTypedOuter<URPGEntity>();
    if (auto *ExpectedStruct = Owner != nullptr ? Owner->GetEntityStruct() : nullptr; ExpectedStruct != nullptr)
    {
        for (auto *Function : TFieldView<UFunction>(Comp->GetClass()) |
                                  ranges::views::filter(ranges::bind_back(
                                      &FRPGComponentInitializerCustomization::IsValidInitFunction, ExpectedStruct)))
        {
            MenuBuilder.AddMenuEntry(Function->GetDisplayNameText(), Function->GetToolTipText(), FSlateIcon(),
                                     FUIAction(FExecuteAction::CreateLambda([this, Function] {
                                         OuterPropertyHandle->NotifyPreChange();

                                         void *RawValue;
                                         OuterPropertyHandle->GetValueData(RawValue);
                                         auto &Initializer = *static_cast<FRPGComponentInitializer *>(RawValue);
                                         Initializer.InitFunction = const_cast<UFunction *>(Function);

                                         OuterPropertyHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
                                     })));
        }
    }

    return MenuBuilder.MakeWidget();
}

FText FRPGComponentInitializerCustomization::GetCurrentFunctionText() const
{
    void *RawValue;
    OuterPropertyHandle->GetValueData(RawValue);
    const auto &Initializer = *static_cast<FRPGComponentInitializer *>(RawValue);
    return Initializer.InitFunction != nullptr ? Initializer.InitFunction->GetDisplayNameText()
                                               : NSLOCTEXT("RPG", "None", "None");
}

bool FRPGComponentInitializerCustomization::IsValidInitFunction(const UFunction *Func,
                                                                const UScriptStruct *ExpectedStruct)
{
    if (Func == nullptr || ExpectedStruct == nullptr)
        return false;
    if (Func->GetReturnProperty() != nullptr)
        return false;

    int32 ParamCount = 0;
    const FStructProperty *StructParam = nullptr;

    for (auto *Param : TFieldView<FProperty>(Func))
    {
        ++ParamCount;
        if (Param->HasAnyPropertyFlags(CPF_Parm) && !Param->HasAnyPropertyFlags(CPF_OutParm))
        {
            StructParam = CastField<FStructProperty>(Param);
        }
    }

    return ParamCount == 1 && StructParam && StructParam->Struct == ExpectedStruct;
}