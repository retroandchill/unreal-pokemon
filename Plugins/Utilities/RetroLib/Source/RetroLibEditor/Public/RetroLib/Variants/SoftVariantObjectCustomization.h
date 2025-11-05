// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "DetailWidgetRow.h"
#include "PropertyCustomizationHelpers.h"
#include "RetroLib/Optionals/OrElseGet.h"
#include "RetroLib/Optionals/Transform.h"
#include "RetroLib/Utils/Construct.h"
#include "RetroLib/Variants/SoftVariantObject.h"

namespace Retro {
    /**
     * The customization for any struct that is an alias for TVariantObject
     * @tparam T The type of variant that is being customized
     */
    template <typename T>
        requires SoftVariantObjectStruct<T>
    class TSoftVariantObjectCustomization : public IPropertyTypeCustomization {
      public:
        static TSharedRef<IPropertyTypeCustomization> MakeInstance() {
            return MakeShared<TSoftVariantObjectCustomization>();
        }

        void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow &HeaderRow,
                             IPropertyTypeCustomizationUtils &StructCustomizationUtils) override {
            SoftVariantObjectPropertyHandle = StructPropertyHandle;
            // clang-format off
			HeaderRow.NameContent()
				[
					StructPropertyHandle->CreatePropertyNameWidget()
				]
				.ValueContent()
				[
				    SNew(SObjectPropertyEntryBox)
                        .ObjectPath(this, &TSoftVariantObjectCustomization::GetCurrentAssetPath)
                        .AllowedClass(UObject::StaticClass())
                        .OnShouldFilterAsset(this, &TSoftVariantObjectCustomization::IsValidClass)
                        .OnObjectChanged(this, &TSoftVariantObjectCustomization::OnAssetSelected)
                        .DisplayUseSelected(true)
                        .DisplayBrowse(true)
                        .DisplayThumbnail(true)
                        .ThumbnailPool(StructCustomizationUtils.GetThumbnailPool())
				];
            // clang-format on
        }

        void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder &ChildBuilder,
                               IPropertyTypeCustomizationUtils &StructCustomizationUtils) override {
            // No child customization
        }

      private:
        T &GetSoftVariantObject() const {
            void *StructData;
            const auto Result = SoftVariantObjectPropertyHandle->GetValueData(StructData);
            check(Result == FPropertyAccess::Success)
            check(StructData != nullptr)
            return *static_cast<T *>(StructData);
        }

        FString GetCurrentAssetPath() const {
            // clang-format off
            return GetSoftVariantObject().ToSoftObjectPtr().ToString();
            // clang-format on
        }

        bool IsValidClass(const FAssetData &Asset) const {
            return !T::FHardReference::IsValidType(Asset.GetClass(EResolveClass::Yes));
        }

        void OnAssetSelected(const FAssetData &InAsset) const {
            SoftVariantObjectPropertyHandle->NotifyPreChange();
            GetSoftVariantObject().Set(TSoftObjectPtr(InAsset.GetSoftObjectPath()));
            SoftVariantObjectPropertyHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
        }

        TSharedPtr<IPropertyHandle> SoftVariantObjectPropertyHandle;
    };
} // namespace Retro
