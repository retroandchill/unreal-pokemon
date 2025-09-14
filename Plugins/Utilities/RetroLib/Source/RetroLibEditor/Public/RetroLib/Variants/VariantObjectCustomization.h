// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DetailWidgetRow.h"
#include "PropertyCustomizationHelpers.h"
#include "RetroLib/Optionals/IfPresent.h"
#include "RetroLib/Optionals/OrElseGet.h"
#include "RetroLib/Optionals/Transform.h"
#include "RetroLib/Utils/Construct.h"
#include "RetroLib/Variants/VariantObjectStruct.h"

namespace Retro
{
    /**
     * The customization for any struct that is an alias for TVariantObject
     * @tparam T The type of variant that is being customized
     */
    template <typename T>
        requires VariantObjectStruct<T>
    class TVariantObjectCustomization : public IPropertyTypeCustomization
    {
      public:
        static TSharedRef<IPropertyTypeCustomization> MakeInstance()
        {
            return MakeShared<TVariantObjectCustomization>();
        }

        void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow &HeaderRow,
                             IPropertyTypeCustomizationUtils &StructCustomizationUtils) override
        {
            VariantObjectPropertyHandle = StructPropertyHandle;
            // clang-format off
			HeaderRow.NameContent()
				[
					StructPropertyHandle->CreatePropertyNameWidget()
				]
				.ValueContent()
				[
				    SNew(SObjectPropertyEntryBox)
				        .ObjectPath(this, &TVariantObjectCustomization::GetCurrentAssetPath)
                        .AllowedClass(UObject::StaticClass())
				        .OnShouldFilterAsset(this, &TVariantObjectCustomization::IsValidClass)
                        .OnObjectChanged(this, &TVariantObjectCustomization::OnAssetSelected)
                        .DisplayUseSelected(true)
                        .DisplayBrowse(true)
						.DisplayThumbnail(true)
					    .ThumbnailPool(StructCustomizationUtils.GetThumbnailPool())
				];
            // clang-format on
        }

        void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder &ChildBuilder,
                               IPropertyTypeCustomizationUtils &StructCustomizationUtils) override
        {
            // No child customization
        }

      private:
        T &GetVariantObject() const
        {
            void *StructData;
            const auto Result = VariantObjectPropertyHandle->GetValueData(StructData);
            check(Result == FPropertyAccess::Success)
            check(StructData != nullptr)
            return *static_cast<T *>(StructData);
        }

        FString GetCurrentAssetPath() const
        {
            // clang-format off
            return GetVariantObject().TryGet() |
                   Optionals::Transform([](const UObject &Obj) { return Obj.GetPathName(); }) |
                   Optionals::OrElseGet(Construct<FString>);
            // clang-format on
        }

        bool IsValidClass(const FAssetData &Asset) const
        {
            return !T::IsValidType(Asset.GetClass(EResolveClass::Yes));
        }

        void OnAssetSelected(const FAssetData &InAsset) const
        {
            VariantObjectPropertyHandle->NotifyPreChange();
            GetVariantObject().Set(InAsset.GetAsset());
            VariantObjectPropertyHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
        }

        TSharedPtr<IPropertyHandle> VariantObjectPropertyHandle;
    };
} // namespace Retro
