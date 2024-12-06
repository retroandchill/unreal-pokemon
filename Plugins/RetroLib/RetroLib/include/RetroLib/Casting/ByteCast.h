/**
 * @file ByteCast.h
 * @brief Cast type for converting between byte arrays and pointers.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include <bit>
#include <type_traits>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro {
    /**
     * Represents a type we see as a valid opaque byte pointer.
     *
     * @tparam T The test type
     */
    template <typename T>
    concept ValidByteType = std::same_as<T, std::byte> || std::same_as<T, uint8_t> || std::same_as<T, int8_t> || std::same_as<T, void>;

    template <typename T>
    /**
     * @class ByteCast
     * @brief A utility class for casting various data types to byte representations.
     *
     * TByteCast provides functionality to cast different data types such as integers,
     * floats, and other primitives to their corresponding byte representations. This is
     * useful in scenarios where low-level data manipulation or communication protocols
     * that require data in byte form are involved.
     *
     * Key features:
     * - Cast integers (int, long, etc.) to byte arrays.
     * - Cast floating-point numbers (float, double) to byte arrays.
     * - Ensure proper endianess during the casting process.
     * - Utility methods to handle casting of custom objects, if needed.
     */
    struct ByteCast {
        /**
         * @brief Casts a byte pointer to a specified data type.
         *
         * This operator takes a pointer to a byte array and reinterprets it as a pointer to
         * the specified data type `T`, then returns the dereferenced value. This is useful
         * for low-level data manipulation where you need to convert a byte array back to
         * a structured data type.
         *
         * @param Bytes A pointer to a byte array that will be reinterpreted as a pointer to type `T`.
         * @return The value of the byte array reinterpreted as a value of type `T`.
         */
        template <typename U>
            requires ValidByteType<U>
        constexpr decltype(auto) operator()(U *Bytes) const {
            return *std::bit_cast<T *>(Bytes);
        }
    };

    /**
     * @brief A constant instance of ByteCast used to cast various data types to byte representations.
     *
     * byte_cast is an instance of ByteCast, serving as a utility for converting different data types such as integers,
     * floats, and other primitives into their corresponding byte representations. This is particularly useful for
     * scenarios that require low-level data manipulation or protocols that handle data in byte form.
     *
     * Key features:
     * - Converts integers (int, long, etc.) to byte arrays.
     * - Converts floating-point numbers (float, double) to byte arrays.
     * - Ensures proper endianess during the conversion process.
     * - Provides additional functionality for custom object conversions if necessary.
     */
    RETROLIB_EXPORT template <typename T>
    constexpr ByteCast<T> byte_cast;
}