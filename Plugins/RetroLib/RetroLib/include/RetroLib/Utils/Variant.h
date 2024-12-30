/**
 * @file Variant.h
 * @brief Helper methods and concepts for std::variant.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */

#pragma once

#if !RETROLIB_WITH_MODULES
#include <variant>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {

    /**
     * @struct IndexedElement
     * @brief A wrapper structure that holds a pointer to an element of type T.
     *
     * This structure is used for managing and accessing a referenced object of type T
     * via a pointer. The object reference is passed to the structure upon construction,
     * and can be accessed through the `get` method.
     *
     * @tparam T The type of the element being referenced.
     */
    RETROLIB_EXPORT template <typename T, size_t I>
    struct IndexedElement {
      private:
        std::add_pointer_t<T> Ptr;

      public:
        /**
         * @brief Constructs an IndexedElement with a reference to an object.
         *
         * This constructor initializes the IndexedElement with a reference
         * to the given object. The address of the object is stored internally.
         *
         * @param Ptr A reference to the object to be indexed.
         */
        constexpr explicit IndexedElement(T &Ptr) : Ptr(std::addressof(Ptr)) {
        }

        /**
         * @brief Retrieves a reference to the object being pointed to.
         *
         * This method provides access to the object by dereferencing the
         * stored pointer. It guarantees no exception will be thrown.
         *
         * @return A reference to the object of type T.
         */
        constexpr T &Get() const noexcept {
            return *Ptr;
        }
    };

    /**
     * @brief Represents an indexed element with an associated object reference.
     *
     * This struct provides a mechanism to store and retrieve references to
     * objects of a given type while maintaining an associated index.
     *
     * @tparam T The type of the object being referenced, represented as an rvalue reference.
     * @tparam I The type associated with the index.
     */
    RETROLIB_EXPORT template <typename T, size_t I>
    struct IndexedElement<T &&, I> {
      private:
        T *Ptr;

      public:
        /**
         * @brief Constructs an IndexedElement with a reference to an object.
         *
         * This explicit constructor initializes the IndexedElement with a reference
         * to the provided object, utilizing perfect forwarding to ensure efficient
         * initialization. The address of the object is internally stored.
         *
         * @param Ptr An rvalue reference to the object to be indexed.
         */
        constexpr explicit IndexedElement(T &&Ptr) : Ptr(std::addressof(Ptr)) {
        }

        /**
         * @brief Retrieves the object being referenced and transfers ownership.
         *
         * This function returns an rvalue reference to the object stored internally,
         * transferring ownership of the referenced object. The operation is noexcept.
         *
         * @return A rvalue reference to the object of type T.
         */
        constexpr T &&Get() const noexcept {
            return std::move(*Ptr);
        }
    };

    /**
     * @brief Specialization of IndexedElement for void type.
     *
     * This specialization of the IndexedElement struct is for the case where
     * the indexed type is void. It provides a no-operation implementation
     * of the get() method.
     *
     * @tparam I The type of the index associated with the void element.
     */
    RETROLIB_EXPORT template <size_t I>
    struct IndexedElement<void, I> {
        /**
         * @brief Retrieves a value or performs an operation.
         *
         * This method is a constexpr, noexcept function that does not perform
         * any operation. It is designed to be called in constant expressions and
         * guarantees no exceptions will be thrown.
         */
        constexpr void Get() const noexcept {
            // No operation needed for this one
        }
    };

    /**
     * @brief Invokes a visitor function with an IndexedElement created from a variant and its index.
     *
     * This helper function forwards the specified visitor and variant, then invokes the visitor
     * with an IndexedElement containing an element from the variant and its index.
     *
     * @param Visitor A callable entity (function, lambda, etc.) to be applied to the IndexedElement.
     * @param Variant An instance of a variant type that is used to extract an indexed element.
     * @return The result of invoking the visitor with the IndexedElement.
     */
    template <size_t I, typename F, typename V>
    constexpr decltype(auto) VisitIndexHelper(F &&Visitor, V &&Variant) {
        return std::invoke(std::forward<F>(Visitor),
                           IndexedElement<std::variant_alternative_t<I, std::remove_reference_t<V>>, I>(
                               std::get<I>(std::forward<V>(Variant))));
    }

    /**
     * @brief Visits the active alternative of a variant by index.
     *
     * This function dispatches a visitor to the active alternative of the given variant.
     * It determines the index of the active alternative at compile time and calls
     * the corresponding helper function using a function pointer array.
     *
     * @tparam F The type of the visitor function or callable object.
     * @tparam V The type of the variant object to be visited.
     * @param Visitor The visitor object or function to apply to the active alternative of the variant.
     * @param Variant The variant object whose active alternative is being visited.
     * @return The result of invoking the visitor on the active alternative of the variant.
     */
    RETROLIB_EXPORT template <typename F, typename V>
    constexpr decltype(auto) VisitIndex(F &&Visitor, V &&Variant) {
        constexpr auto N = std::variant_size_v<std::remove_cvref_t<V>>;
        constexpr auto FuncPtrs = []<size_t... I>(std::index_sequence<I...>) {
            return std::array{&VisitIndexHelper<I, F, V>...};
        }(std::make_index_sequence<N>{});
        return FuncPtrs[Variant.index()](std::forward<F>(Visitor), std::forward<V>(Variant));
    }

    /**
     * @brief Visits the active alternative of a variant by index.
     *
     * This function dispatches a visitor to the active alternative of the given variant.
     * It determines the index of the active alternative at compile time and calls
     * the corresponding helper function using a function pointer array.
     *
     * @tparam F The type of the visitor function or callable object.
     * @tparam V The type of the variant object to be visited.
     * @param Visitor The visitor object or function to apply to the active alternative of the variant.
     * @param Variant The variant object whose active alternative is being visited.
     * @return The result of invoking the visitor on the active alternative of the variant.
     */
    RETROLIB_EXPORT template <typename R, typename F, typename V>
    constexpr R VisitIndex(F &&Visitor, V &&Variant) {
        constexpr auto N = std::variant_size_v<std::remove_cvref_t<V>>;
        constexpr auto FuncPtrs = []<size_t... I>(std::index_sequence<I...>) {
            return std::array{&VisitIndexHelper<I, F, V>...};
        }(std::make_index_sequence<N>{});
        return FuncPtrs[Variant.index()](std::forward<F>(Visitor), std::forward<V>(Variant));
    }

} // namespace retro