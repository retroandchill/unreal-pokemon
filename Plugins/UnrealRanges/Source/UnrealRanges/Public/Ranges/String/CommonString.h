// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Blueprints/Properties.h"
#include <variant>

namespace UE::Ranges {

    /**
     * Concept that defines if a type can be converted to a common string.
     * @tparam T The potential string type
     */
    template <typename T>
    concept Stringable = std::convertible_to<T, FStringView> || std::same_as<std::remove_cvref_t<T>, FName> ||
                         std::same_as<std::remove_cvref_t<T>, FText>;

    /**
     * Type union for any possible types that can be rendered as a string. This allows for the handling of cases where
     * an r-value reference to either {@link FString} or {@link FText} is passed through, as well as all other cases
     * where {@link FStringView} can be used.
     */
    using FCommonString = std::variant<FString, FStringView, FText>;

    /**
     * Converts the given value into a common string representation.
     *
     * This function takes a value of type T and converts it to a variant of FString, FStringView,
     * or FText, based on the type of the value. The return type will be FCommonString.
     *
     * @param Value The value to be converted. This can be an instance of FName, FText, FString or any type
     * that can be converted to FStringView.
     * @return A variant containing the string representation of the value.
     */
    template <typename T>
        requires Stringable<T>
    FCommonString GetString(T &&Value) {
        if constexpr (std::same_as<std::remove_cvref_t<T>, FName>) {
            return FCommonString(Value.ToString());
        } else if constexpr (std::same_as<std::remove_cvref_t<T>, FText>) {
            if constexpr (std::is_lvalue_reference_v<T>) {
                return FCommonString(FStringView(Value.ToString()));
            } else {
                return FCommonString(Forward<T>(Value));
            }
        } else if constexpr (std::same_as<std::remove_cvref_t<T>, FString>) {
            if constexpr (std::is_lvalue_reference_v<T>) {
                return FCommonString(FStringView(Forward<T>(Value)));
            } else {
                return FCommonString(Forward<T>(Value));
            }
        } else {
            return FCommonString(FStringView(Value));
        }
    }

    /**
     * Extracts a common string representation from a given property.
     *
     * This function uses a property visitor to retrieve the property's value and then
     * converts it into a common string format, which can be one of FString, FStringView,
     * or FText, based on the type of the value.
     *
     * @param Property A pointer to the property from which to extract the string.
     * @param Data A pointer to the data containing the property value.
     * @return A variant containing the string representation of the property value.
     * @throws FInvalidArgumentException if the property type is not found within the predefined set of classes.
     */
    UNREALRANGES_API FCommonString ExtractCommonStringFromProperty(const FProperty *Property, const uint8 *Data);

    /**
     * Retrieves a view of the given string.
     *
     * This function converts a string of type T into an FStringView. If the input string
     * is of type FText, it converts it to FStringView using the ToString method.
     * Otherwise, it treats the input directly as FStringView.
     *
     * @param String The string to be converted. This value can be an instance of FText, FString, or FStringView.
     * @return An FStringView representation of the string.
     */
    template <typename T>
        requires std::constructible_from<FCommonString, T> && (!std::same_as<std::remove_cvref_t<T>, FCommonString>)
    FStringView GetStringView(T &&String) {
        if constexpr (std::same_as<std::remove_cvref_t<T>, FText>) {
            return String.ToString();
        } else {
            return String;
        }
    }

    /**
     * Retrieves a view of the string from an FCommonString variant.
     *
     * This function visits the FCommonString variant and extracts a string view from it,
     * which can manage different string types such as FString, FStringView, and FText.
     *
     * @param String The FCommonString variant containing the string data.
     * @return A FStringView representing the view of the string.
     */
    UNREALRANGES_API FStringView GetStringView(const FCommonString &String);

    /**
     * Visitor for any property can can contain a value that can easily be converted to a string.
     */
    using FStringPropertyVisitor = TPropertyVisitor<FNameProperty, FStrProperty, FTextProperty>;

} // namespace UE::Ranges
