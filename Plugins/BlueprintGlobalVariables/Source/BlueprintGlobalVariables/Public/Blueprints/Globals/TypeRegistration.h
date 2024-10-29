// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Blueprints/BlueprintRuntimeUtils.h"
#include "Ranges/Concepts/Enums.h"
#include "Ranges/Concepts/Structs.h"
#include <any>

namespace UE::Blueprints {
    class IVariableTypeRegistration {
    public:
        virtual ~IVariableTypeRegistration() = default;

        virtual const std::type_info &GetTypeInfo() const = 0;
        virtual FName GetTypeName() const = 0;
        virtual std::any CreateNew() const = 0;
        virtual void EmplaceInto(std::any &Value, const FProperty &Property, uint8 *Data) const = 0;
    };

    template <typename T>
        requires std::derived_from<T, FProperty>
    const T &TryCastProperty(const FProperty &Property) {
        auto CastProperty = CastField<const T>(&Property);
        if (CastProperty == nullptr) {
            throw Ranges::FTypeException(EBlueprintExceptionType::AccessViolation,
                                         NSLOCTEXT("BlueprintGlobals", "InvalidPropertyType",
                                                   "Invalid property type!"));
        }

        return *CastProperty;
    }

    template <typename T>
    concept HasCppType = std::derived_from<T, FProperty> && requires
    {
        typename T::TCppType;
    };

    template <typename T>
    class TVariableTypeRegistrationImplBase : public IVariableTypeRegistration {
    public:
        explicit TVariableTypeRegistrationImplBase(FName Data) : TypeName(Data) {
        }

        const std::type_info &GetTypeInfo() const final {
            return typeid(T);
        }

        FName GetTypeName() const final {
            return TypeName;
        }

        std::any CreateNew() const final {
            return std::any(T());
        }

    private:
        FName TypeName;
    };

    template <typename T>
        requires HasCppType<T>
    class TPrimitiveTypeRegistration : public TVariableTypeRegistrationImplBase<typename T::TCppType> {
    public:
        explicit
        TPrimitiveTypeRegistration(FName Data) : TVariableTypeRegistrationImplBase<typename T::TCppType>(Data) {
        }

        void EmplaceInto(std::any &Value, const FProperty &Property, uint8 *Data) const final {
            TryCastProperty<T>(Property);
            void *RawData = Data;
            Value.emplace<typename T::TCppType>(*static_cast<typename T::TCppType *>(RawData));
        }
    };

    template <typename T>
        requires Ranges::UEStruct<T>
    class TStructTypeRegistration : public TVariableTypeRegistrationImplBase<T> {
    public:
        explicit TStructTypeRegistration(FName Data) : TVariableTypeRegistrationImplBase<T>(Data) {
        }

        void EmplaceInto(std::any &Value, const FProperty &Property, uint8 *Data) const final {
            auto &StructProperty = TryCastProperty<FStructProperty>(Property);
            Ranges::ValidateStructsMatch(StructProperty, Ranges::GetScriptStruct<T>());
            void *RawData = Data;
            Value.emplace<T>(*static_cast<T*>(RawData));
        }
    };

    template <typename T>
        requires Ranges::UEEnum<T>
    class TEnumTypeRegistration : public TVariableTypeRegistrationImplBase<T> {
    public:
        explicit TEnumTypeRegistration(FName Data) : TVariableTypeRegistrationImplBase<T>(Data) {
        }

        void EmplaceInto(std::any &Value, const FProperty &Property, uint8 *Data) const final {
            auto &EnumProperty = TryCastProperty<FEnumProperty>(Property);
            Ranges::ValidateStructsMatch(EnumProperty, StaticEnum<T>());
            void *RawData = Data;
            Value.emplace<T>(*static_cast<T*>(RawData));
        }
    };
}