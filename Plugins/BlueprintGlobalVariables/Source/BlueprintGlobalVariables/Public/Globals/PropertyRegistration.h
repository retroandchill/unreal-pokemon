// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PropertyTraits.h"
#include "Ranges/Blueprints/BlueprintRuntimeUtils.h"
#include "Ranges/Concepts/Enums.h"
#include "Ranges/Concepts/Structs.h"
#include <any>

namespace Blueprint::Globals {
    /**
     * @interface IPropertyTrait
     * @brief Interface for defining property traits in objects.
     *
     * This interface provides a contract for classes that describe properties
     * and their behaviors. Implementing classes are expected to provide
     * concrete definitions for property-specific actions and metadata.
     */
    class IPropertyTrait {
      public:
        virtual ~IPropertyTrait() = default;

        /**
         * @brief Copies data from a property into a storage.
         *
         * This pure virtual function is responsible for copying the specified property's data
         * into a provided storage. Implementing classes should define the specifics of how
         * the data is to be copied.
         *
         * @param Storage The target storage where the property data will be copied to.
         * @param Property The property from which data is to be copied.
         * @param Data A pointer to the data that will be copied from the property.
         */
        virtual void CopyFromProperty(std::any &Storage, const FProperty &Property, const uint8 *Data) const = 0;

        /**
         * @brief Copies data into a property from a storage.
         *
         * This pure virtual function handles copying data from a given storage into
         * the specified property. Implementing classes must define the mechanism for
         * transferring data into the property.
         *
         * @param Storage The source storage that contains the data to be copied.
         * @param Property The target property where data will be copied to.
         * @param Data A pointer to the memory location where the data will be stored in the property.
         * @throws UE::Ranges::FVariantException If there is a mismatch between the type inside the storage and the
         * target type
         */
        virtual void CopyToProperty(const std::any &Storage, FProperty &Property, uint8 *Data) const = 0;
    };

    template <typename T>
        requires HasPropertyType<T>
    class TPrimitivePropertyTrait : public IPropertyTrait {
      public:
        void CopyFromProperty(std::any &Storage, const FProperty &Property, const uint8 *Data) const final {
            if constexpr (HasStaticPropertyGetter<T>) {
                Storage.emplace<T>(typename T::GetPropertyValue(Data));
            } else {
                auto TypedProperty = CastFieldChecked<TPropertyType<T>>(&Property);
                Storage.emplace<T>(TypedProperty->GetPropertyValue(Data));
            }
        }

        void CopyToProperty(const std::any &Storage, FProperty &Property, uint8 *Data) const {
            try {
                if constexpr (HasStaticPropertyGetter<T>) {
                    typename T::SetPropertyValue(Data, std::any_cast<const T &>(Storage));
                } else {
                    auto TypedProperty = CastFieldChecked<TPropertyType<T>>(&Property);
                    TypedProperty->SetPropertyValue(Data, std::any_cast<const T &>(Storage));
                }
            } catch (const std::bad_any_cast &E) {
                throw new UE::Ranges::FVariantException(EBlueprintExceptionType::AccessViolation,
                                                        FText::FromStringView(ANSI_TO_TCHAR(E.what())));
            }
        }
    };

    template <typename T>
        requires HasPropertyType<T> && UE::Ranges::UEStruct<T>
    class TStructPropertyTrait : public IPropertyTrait {
      public:
        void CopyFromProperty(std::any &Storage, const FProperty &Property, const uint8 *Data) const final {
            auto ScriptStruct = UE::Ranges::GetScriptStruct<T>();
            auto StructProperty = CastFieldChecked<FStructProperty>(&Property);
            UE::Ranges::ValidateStructsMatch(*StructProperty, ScriptStruct);
            const void *RawData = Data;
            Storage.emplace<T>(*static_cast<const T *>(RawData));
        }

        void CopyToProperty(const std::any &Storage, FProperty &Property, uint8 *Data) const {
            auto ScriptStruct = UE::Ranges::GetScriptStruct<T>();
            auto StructProperty = CastFieldChecked<FStructProperty>(&Property);
            UE::Ranges::ValidateStructsMatch(*StructProperty, ScriptStruct);

            try {
                StructProperty->SetValue_InContainer(Data, &std::any_cast<const T &>(Storage));
            } catch (const std::bad_any_cast &E) {
                throw new UE::Ranges::FVariantException(EBlueprintExceptionType::AccessViolation,
                                                        FText::FromStringView(ANSI_TO_TCHAR(E.what())));
            }
        }
    };

    template <typename T>
        requires HasPropertyType<T> && UE::Ranges::UEEnum<T>
    class TEnumPropertyTrait : public IPropertyTrait {
      public:
        void CopyFromProperty(std::any &Storage, const FProperty &Property, const uint8 *Data) const final {
            auto Enum = StaticEnum<T>();
            auto EnumProperty = CastFieldChecked<FEnumProperty>(&Property);
            UE::Ranges::ValidateEnumsMatch(*EnumProperty, Enum);
            const void *RawData = Data;
            Storage.emplace<T>(*static_cast<const T *>(RawData));
        }

        void CopyToProperty(const std::any &Storage, FProperty &Property, uint8 *Data) const {
            auto Enum = StaticEnum<T>();
            auto EnumProperty = CastFieldChecked<FEnumProperty>(&Property);
            UE::Ranges::ValidateEnumsMatch(*EnumProperty, Enum);

            try {
                EnumProperty->SetValue_InContainer(Data, &std::any_cast<const T &>(Storage));
            } catch (const std::bad_any_cast &E) {
                throw new UE::Ranges::FVariantException(EBlueprintExceptionType::AccessViolation,
                                                        FText::FromStringView(ANSI_TO_TCHAR(E.what())));
            }
        }
    };

    class BLUEPRINTGLOBALVARIABLES_API FPropertyTraitRegistry {
        FPropertyTraitRegistry() = default;
        ~FPropertyTraitRegistry() = default;

      public:
        UE_NONCOPYABLE(FPropertyTraitRegistry)

        static FPropertyTraitRegistry &Get();

        template <typename T>
            requires HasPropertyType<T>
        void AddPropertyType() {
            if constexpr (UE::Ranges::UEStruct<T>) {
                UScriptStruct *StructType = UE::Ranges::GetScriptStruct<T>();
                PropertyTraits.Emplace(StructType->GetFName(), MakeUnique<TStructPropertyTrait<T>>());
            } else if constexpr (UE::Ranges::UEEnum<T>) {
                UEnum *EnumType = StaticEnum<T>();
                PropertyTraits.Emplace(EnumType->GetFName(), MakeUnique<TEnumPropertyTrait<T>>());
            } else {
                FFieldClass *FieldType = TPropertyType<T>::StaticClass();
                PropertyTraits.Emplace(FieldType->GetFName(), MakeUnique<TPrimitivePropertyTrait<T>>());
            }
        }

      private:
        TMap<FName, TUniquePtr<IPropertyTrait>> PropertyTraits;
    };

    template <typename T>
        requires HasPropertyType<T>
    bool RegisterProperty() {
        FCoreDelegates::OnPostEngineInit.AddLambda([] { FPropertyTraitRegistry::Get().AddPropertyType<T>(); });
        return true;
    }
} // namespace Blueprint::Globals

#define DEFINE_PROPERTY_REGISTRATION(TypeName)                                                                         \
    static const bool __##TypeName##__Registered = Blueprint::Globals::RegisterProperty<TypeName>()