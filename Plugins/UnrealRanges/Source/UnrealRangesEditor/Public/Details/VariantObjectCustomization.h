// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "DetailWidgetRow.h"
#include "Ranges/Variants/VariantObject.h"

namespace UE::Ranges {
    /**
     * The customization for any struct that is an alias for TVariantObject
     * @tparam T The type of variant that is being customized
     */
    template <typename T>
        requires VariantObjectStruct<T>
    class TVariantObjectCustomization : public IPropertyTypeCustomization {
    public:
        static TSharedRef<IPropertyTypeCustomization> MakeInstance() {
            return MakeShared<TVariantObjectCustomization>();
        }

        void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow,
            IPropertyTypeCustomizationUtils& StructCustomizationUtils) override {
            auto WrappedProperty = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(T, ContainedObject));
            // clang-format off
            HeaderRow.NameContent()
                [
                    StructPropertyHandle->CreatePropertyNameWidget()
                ]
                .ValueContent()
                [
                    WrappedProperty->CreatePropertyValueWidget()
                ];
            // clang-format on
            
            WrappedProperty->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda([StructPropertyHandle] {
                void* StructData;
                if (const auto Result = StructPropertyHandle->GetValueData(StructData); Result != FPropertyAccess::Success) {
                    return;
                }

                check(StructData != nullptr)
                auto AsVariant = static_cast<T*>(StructData);
                AsVariant->TypeIndex = AsVariant->GetTypeIndex(AsVariant->ContainedObject).GetValue();
            }));
        }
        
        void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& ChildBuilder,
            IPropertyTypeCustomizationUtils& StructCustomizationUtils) override {
            // No child customization
        }
    };
}
