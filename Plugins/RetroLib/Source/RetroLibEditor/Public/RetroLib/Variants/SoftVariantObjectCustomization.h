// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "DetailWidgetRow.h"
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
			auto WrappedProperty = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(T, Ptr));
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
		}

		void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder &ChildBuilder,
							   IPropertyTypeCustomizationUtils &StructCustomizationUtils) override {
			// No child customization
		}
	};
} // namespace UE::Ranges

