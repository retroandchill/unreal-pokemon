// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Exceptions/InvalidArgumentException.h"

THIRD_PARTY_INCLUDES_START
#include <range/v3/algorithm/find_if.hpp>
THIRD_PARTY_INCLUDES_END

namespace UE::Ranges {

    /**
     * Concept used to check that a property get actually get a value out using a static method.
     * 
     * @tparam T The type to validate the concept against.
     */
    template <typename T>
    concept CanGetValue = requires (T&& Property, const void* Data) {
        T::GetPropertyValue(Data);
    };
    

    /**
     * @class TPropertyVisitor
     *
     * @brief A templated class designed to visit and manipulate properties in Unreal Engine.
     *
     * This class is constructed using a specific property type and provides methods to retrieve type indices and property values.
     *
     * @tparam T A list of property types derived from FProperty.
     */
    template <typename... T>
        requires ((std::derived_from<T, FProperty> && CanGetValue<T>) && ...)
    class TPropertyVisitor {
    public:
        /**
         * Constructs a TPropertyVisitor with the provided property, initializing the TypeIndex.
         *
         * @param Property A pointer to the FProperty object to be used for initializing the visitor.
         * @return An instance of TPropertyVisitor with the TypeIndex set based on the provided property.
         * @throws FInvalidArgumentException if the property type is not found within the predefined set of classes.
         */
        explicit TPropertyVisitor(const FProperty* Property) : TypeIndex(GetTypeIndex(Property)) {}


        /**
         * Retrieves the index of the property type from a predefined set of classes.
         *
         * @param Property A pointer to the FProperty object whose type index is to be determined.
         * @return The index of the property type if found within the predefined set of classes.
         * @throws FInvalidArgumentException if the property type is not found within the predefined set of classes.
         */
        static size_t GetTypeIndex(const FProperty* Property) {
            static std::array Classes = { T::StaticClass()... };
            auto ValidClass = ranges::find_if(Classes, [Property](const FFieldClass* Class) { return Property->IsA(Class); });
            if (ValidClass == Classes.end()) {
                throw FInvalidArgumentException(EBlueprintExceptionType::AccessViolation,
                    NSLOCTEXT("TPropertyVisitor", "GetTypeIndex_Error", "Invalid property type!"));
            }
            
            return std::distance(Classes.begin(), ValidClass);
        }

        /**
         * Invokes the appropriate VisitSingle method based on the TypeIndex for the given data and functor.
         *
         * @param Data A pointer to the data to be passed to the VisitSingle method.
         * @param Functor The functor to be invoked with the data.
         * @return The result of invoking the VisitSingle method specified by the TypeIndex with the given data and functor.
         * @tparam F The signature of the functional callback
         */
        template <typename F>
        constexpr decltype(auto) Visit(const uint8* Data, F&& Functor) {
            constexpr std::array Invocations = { &TPropertyVisitor::VisitSingle<T, F>... };
            return ranges::invoke(Invocations[TypeIndex], Data, Forward<F>(Functor));
        }

    private:
        template <typename U, typename F>
            requires (std::same_as<T, U> || ...)
        constexpr static decltype(auto) VisitSingle(const uint8* Data, F&& Functor) {
            return ranges::invoke(Forward<F>(Functor), U::GetPropertyValue(Data));
        }
        
        size_t TypeIndex;
    };
    
}