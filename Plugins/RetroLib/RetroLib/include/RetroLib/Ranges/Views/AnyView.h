/**
 * @file AnyView.h
 * @brief Contains the class definition for the AnyView range adapater class.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/RetroLibMacros.h"
#include "RetroLib/Utils/Polymorphic.h"

#include <memory>
#include <ranges>
#include <type_traits>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Ranges {

    /**
     * @brief A concept to check if a range is compatible with a given type.
     *
     * The CompatibleRange concept ensures that the reference type of a range
     * is convertible to a specified type T. This concept is useful for
     * determining if elements of a range can be used as or transformed into
     * another type, T, thus ensuring compatibility.
     *
     * @tparam R The range type to be checked.
     * @tparam T The type to which the range's reference type should be convertible.
     */
    template <typename R, typename T>
    concept CompatibleRange = std::convertible_to<std::ranges::range_reference_t<R>, T>;

    /**
     * A templated structure that represents a runtime type information (RTTI) tag.
     *
     * This structure is intended to be used as a marker or tag for RTTI purposes
     * in templates where a type-specific identification is required. The
     * `RttiTag` struct is essentially an empty placeholder and does not include
     * any member functions or variables, serving as a compile-time identifier
     * for the specified type. By templating over a type, it allows for the
     * differentiation of various types at compile time without instantiating
     * any actual objects, conserving memory and improving performance.
     *
     * Template Parameters:
     * - `typename`: The type associated with the RTTI tag.
     *
     * The `RttiTag` is typically utilized in type traits, metaprogramming, or
     * other compile-time mechanisms to provide type-specific behavior or
     * optimizations based on the type parameter specified in the template
     * argument.
     */
    template <typename>
    struct RttiTag {};

    /**
     * @class AnyRef
     *
     * @brief A reference wrapper for storing and retrieving object references with runtime type information.
     *
     * The `AnyRef` class provides a mechanism for storing a reference to an object along with its
     * type information using RTTI. This enables type-safe retrieval of the object reference later,
     * ensuring that the stored reference can only be accessed as the correct type.
     *
     * Key Features:
     * - Store a reference to an object of any type.
     * - Retrieve the stored object reference while ensuring type safety through runtime checks.
     *
     * Constructor:
     * - `AnyRef()`: Constructs an empty `AnyRef` instance.
     * - `template<typename T> explicit(false) constexpr AnyRef(T &obj) noexcept`: Constructs an `AnyRef` with a
     * reference to the specified object `obj` and stores its type information.
     *
     * Member Functions:
     * - `template<typename T> T &get() const noexcept`: Returns a reference to the stored object of type `T`. The
     * function will assert if the stored object's type does not match `T` or if the `AnyRef` is empty.
     *
     * Usage of such a class can be particularly beneficial in scenarios where type erasure is needed, or when dynamic
     * polymorphism or containers storing heterogeneous object types are involved.
     */
    class AnyRef {
      public:
        /**
         * @brief Default constructor for the AnyRef class.
         *
         * Constructs an empty AnyRef instance, leaving it in a state that does not
         * contain any object reference or type information. This allows users to
         * create an AnyRef object without immediately assigning it a reference. The
         * AnyRef instance constructed with this constructor should be assigned a valid
         * reference using another mechanism before any attempt to retrieve or use the
         * stored reference.
         */
        AnyRef() = default;

        /**
         * @brief Constructs an AnyRef with a reference to the specified object.
         *
         * This constructor initializes the AnyRef object by storing a reference to the
         * provided object `obj`, along with its runtime type information (RTTI). It enables
         * the holder to later retrieve the reference while ensuring type safety through
         * these stored type details. It guarantees that no operations modifying the
         * referenced object are made during the construction.
         *
         * @tparam T The type of the object being referenced.
         * @param Obj The object to store a reference to.
         *
         * @note The constructor is marked explicit(false), allowing implicit conversions from
         *       object references to AnyRef instances.
         * @note The use of std::addressof ensures a safe retrieval of the pointer to support
         *       cases where the unary & operator might be overloaded for the object's type.
         * @note The constructor is noexcept, guaranteeing that no exceptions will be thrown
         *       during its execution.
         */
        template <typename T>
        explicit(false) constexpr AnyRef(T &Obj) noexcept : Ptr(std::addressof(Obj)), Info(&typeid(RttiTag<T>)) {
        }

        /**
         * @brief Retrieves a reference to the stored object of the specified type.
         *
         * This function returns a reference to the object stored within the `AnyRef` instance, assuming
         * the stored type corresponds to `T`. It asserts the validity of the internal state, checking that
         * both the pointer (`ptr`) and type information (`info`) are not null and that the type held within
         * the `AnyRef` matches the requested type, as indicated by the `RttiTag<T>`.
         *
         * @tparam T The type of the object expected to be stored and retrieved.
         * @return A reference to the stored object of type `T`.
         *
         * @note The call to this function assumes that the internal assertion conditions are met; if not,
         *       the behavior will depend on the assert implementation, which typically involves terminating
         *       the program.
         * @note The use of `const_cast` and `static_cast` allows safely converting the stored `const volatile`
         *       pointer to a modifiable reference of the desired type.
         *
         * @pre The `AnyRef` object should have been initialized and hold a valid reference of type `T`; otherwise
         *      the function contains assertions that may trigger if these conditions are not met.
         * @post Returns a non-const reference to the stored object, allowing modification if desired.
         */
        template <typename T>
        T &get() const noexcept {
            RETROLIB_ASSERT(Ptr != nullptr && Info != nullptr && *Info == typeid(RttiTag<T>));
            return *const_cast<T *>(static_cast<const volatile T *>(Ptr));
        }

      private:
        const volatile void *Ptr = nullptr;
        const std::type_info *Info = nullptr;
    };

    /**
     * @class ErasedView
     *
     * @brief An abstract interface that defines a mechanism to check the end-of-iteration condition
     *        for a collection or sequence using type-erased references.
     *
     * The `ErasedView` class provides an interface that enforces the implementation of an `atEnd`
     * method, which determines if a given object, referenced through the `AnyRef` type-erasure wrapper,
     * is at the end of its iterable sequence or collection. This facilitates the creation of uniform
     * handling of different container types without specific type bindings.
     *
     * Key Features:
     * - Allows handling of different iterable container types through type erasure.
     * - Enables polymorphic iteration end checks without exposing the underlying container type.
     *
     * Typical uses involve creating derived classes that implement specific iteration end conditions for various
     * container types, leveraging type erasure to hide the container's actual type from the users of the interface.
     */
    class ErasedView {
      protected:
        ~ErasedView() = default;

      public:
        /**
         * @brief Determines if the referenced iterable object is at its end.
         *
         * This is a pure virtual function that must be implemented by derived classes
         * to check whether the given object, wrapped by `AnyRef`, has reached the end
         * of its iteration sequence. This function effectively allows polymorphic
         * end-of-iteration checks on type-erased objects.
         *
         * @param Ref A type-erased reference to the object under iteration.
         * @return True if the referenced object is at its end of iteration, false otherwise.
         */
        virtual bool AtEnd(AnyRef Ref) = 0;
    };

    /**
     * @class AnyViewSentinel
     *
     * @brief A sentinel class used to denote the end of an associated view in iteration scenarios.
     *
     * The AnyViewSentinel class is part of an iteration mechanism, typically used alongside
     * an iterator, to signal the end of a view or a data sequence. It holds a reference
     * to an ErasedView object, which defines how the end of the view is determined.
     *
     * @note This class interacts closely with the AnyViewIterator class, specifically for
     * checking when an iteration has reached its end using the `atEnd` method of the
     * ErasedView.
     *
     * @remarks The design of this class includes handling a pointer to an ErasedView object,
     * which must implement necessary logic in its `atEnd()` method. The class is constructed
     * in a manner that prevents undefined behavior from accessing a null pointer by ensuring
     * proper initialization with a valid ErasedView reference.
     */
    RETROLIB_EXPORT class AnyViewSentinel {
      public:
        /**
         * Default constructor for the AnyViewSentinel class.
         *
         * Initializes an instance of AnyViewSentinel with default values.
         */
        AnyViewSentinel() = default;

        /**
         * Constructor for the AnyViewSentinel class.
         *
         * Initializes an AnyViewSentinel object by associating it with the specified ErasedView.
         *
         * @param View A reference to an ErasedView object which will be used to determine the end
         *        of iteration for a sequence. The view provides an interface for checking the iteration
         *        end condition through type-erased references.
         *
         * @note This constructor is declared `constexpr` and `explicit`, ensuring that it can be used
         *       in constant expressions and preventing implicit conversions, respectively. The `noexcept`
         *       specifier indicates that this constructor is guaranteed not to throw exceptions.
         */
        constexpr explicit AnyViewSentinel(ErasedView &View) noexcept : View(&View) {
        }

      private:
        template <typename>
        friend class AnyViewIterator;

        ErasedView *View = nullptr;
    };

    /**
     * @class AnyViewIteratorInterface
     *
     * @brief An interface for iterators used with views, providing basic iteration capabilities.
     *
     * The AnyViewIteratorInterface class defines a generic interface for iterators that can be used
     * within views of type `T`. This interface supports fundamental iteration operations such as
     * reading the current element, checking equality with another iterator, and advancing to the next element.
     *
     * @tparam T The type of elements that the iterator operates over.
     */
    template <typename T>
    class AnyViewIteratorInterface {
      public:
        /**
         * @brief Virtual destructor for the AnyViewIteratorInterface.
         *
         * Ensures that derived classes can be properly destructed when an
         * object is deleted through a pointer to this interface.
         */
        virtual ~AnyViewIteratorInterface() = default;

        /**
         * @brief Returns a reference to the iterator itself.
         *
         * This pure virtual function is intended to be implemented by derived
         * classes to provide a reference to the iterator object. The reference
         * encapsulated within an AnyRef offers access to the iteration utilities
         * or related functionalities associated with the iterator.
         *
         * @return An AnyRef reference to the iterator object.
         */
        virtual AnyRef Iterator() const = 0;

        /**
         * @brief Provides access to the current element in the iteration sequence.
         *
         * This pure virtual function must be implemented by derived classes to return
         * the current element of type `T` in the sequence being iterated. The function
         * defines how to obtain the current element, and the specifics are determined
         * by the implementation in the subclass.
         *
         * @return The element of type `T` at the current position in the iteration.
         */
        virtual T Read() const = 0;

        /**
         * @brief Compares this iterator with another to determine equality.
         *
         * This pure virtual function must be implemented by derived classes
         * to check if the current iterator is equivalent to the specified
         * `other` iterator. The criteria for equality depend on the specific
         * implementation in the concrete subclass.
         *
         * @param other An instance of AnyViewIteratorInterface to compare against.
         * @return True if the iterators are considered equal, false otherwise.
         */
        virtual bool Equal(const AnyViewIteratorInterface &other) const = 0;

        /**
         * @brief Advances the iterator to the next element in the sequence.
         *
         * This pure virtual function must be implemented by derived classes
         * to move the iterator to the next element. The exact behavior,
         * including what constitutes advancing, is defined by the implementation
         * in each subclass.
         */
        virtual void Next() = 0;
    };

    /**
     * @class AnyViewIteratorImpl
     *
     * @brief An implementation of the AnyViewIteratorInterface for generic iterators.
     *
     * The AnyViewIteratorImpl class provides a concrete implementation of the
     * AnyViewIteratorInterface, allowing for iteration over any type that satisfies
     * the requirements of a forward iterator. It adapts a given iterator type `I` to
     * the interface expected for views, enabling iteration operations such as reading
     * the current element, checking equality, and advancing the iterator.
     *
     * @tparam I The type of the underlying iterator this implementation wraps.
     * @tparam T The type of elements that the iterator operates over, defaulted to
     *           the value type of the iterator `I`.
     *
     * Constructor:
     * - `template <typename A> requires std::same_as<std::decay_t<A>, I> && (!std::same_as<std::decay_t<A>,
     * AnyViewIteratorImpl>) constexpr explicit AnyViewIteratorImpl(A&& iter)`: Constructs the iterator implementation
     * with a given iterator. This constructor is enabled for types that decay to `I` and are not `AnyViewIteratorImpl`.
     *
     * - `constexpr explicit AnyViewIteratorImpl(std::nullptr_t) requires std::is_pointer_v<I>`: Constructs an iterator
     *   implementation for pointer types with a null pointer.
     *
     * Member Functions:
     * - `AnyRef iterator() const override`: Returns a reference to the underlying iterator.
     *
     * - `T read() const override`: Returns the current element pointed to by the iterator.
     *
     * - `bool equal(const AnyViewIteratorInterface<T>& other) const override`: Compares this iterator with another
     *   to determine equality. It checks if the underlying iterators are equivalent.
     *
     * - `void next() override`: Advances the iterator to point to the next element in the sequence.
     */
    template <typename I, typename T = std::iter_value_t<I>>
    class AnyViewIteratorImpl : public AnyViewIteratorInterface<T> {
      public:
        /**
         * @brief Constructs an AnyViewIteratorImpl with a specified iterator.
         *
         * This template constructor initializes an AnyViewIteratorImpl instance with the given iterator.
         * It only accepts types that decay to the specified type `I` and ensures the type is not
         * an instance of AnyViewIteratorImpl itself. This is used to adapt various iterator types to the
         * AnyViewIteratorInterface, allowing for unified interaction through the view interface.
         *
         * @tparam A The type of the iterator being adapted, which must be the same as `I` after decay
         *           and not be an AnyViewIteratorImpl.
         *
         * @param Iter The iterator instance to be wrapped by AnyViewIteratorImpl.
         */
        template <typename A>
            requires std::same_as<std::decay_t<A>, I> && (!std::same_as<std::decay_t<A>, AnyViewIteratorImpl>)
        constexpr explicit AnyViewIteratorImpl(A &&Iter) : Iter(std::forward<A>(Iter)) {
        }

        /**
         * @brief Constructs a null AnyViewIteratorImpl for pointer types.
         *
         * This constructor initializes an AnyViewIteratorImpl instance with a null
         * value, applicable only for types where `I` is a pointer. It allows
         * handling of iterators in a way that naturally accommodates null pointers.
         *
         * @requires The template parameter `I` must be a pointer type as determined
         *           by `std::is_pointer_v<I>`.
         */
        constexpr explicit AnyViewIteratorImpl(std::nullptr_t)
            requires std::is_pointer_v<I>
            : Iter(nullptr) {
        }

        AnyRef Iterator() const override {
            return AnyRef(Iter);
        }

        T Read() const override {
            return *Iter;
        }

        bool Equal(const AnyViewIteratorInterface<T> &other) const override {
#ifdef RTTI_ENABLED
            RETROLIB_ASSERT(typeid(*this) == typeid(other));
#endif
            return Iter == static_cast<const AnyViewIteratorImpl<I, T> &>(other).Iter;
        }

        void Next() override {
            ++Iter;
        }

      private:
        I Iter;
    };

    /**
     * @class AnyViewIterator
     *
     * @brief A type-erased, polymorphic iterator for generic container views.
     *
     * The AnyViewIterator class provides a mechanism to iterate over various
     * types of containers or views without knowing the specifics of the underlying
     * iterator type. It allows for operations such as equality comparison,
     * dereferencing to get the current element, and incrementing to move
     * to the next element. This is achieved by delegating the actual operations
     * to an implementation of the AnyViewIteratorInterface.
     *
     * @tparam T The type of elements that the iterator operates over.
     */
    RETROLIB_EXPORT template <typename T>
    class AnyViewIterator {
      public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;

        /**
         * @brief Default constructor for AnyViewIterator.
         *
         * Initializes an AnyViewIterator object with default settings.
         * This constructor does not set any initial state or bind the iterator
         * to any particular view or data source.
         */
        AnyViewIterator() = default;

        /**
         * @brief Constructs an AnyViewIterator from a given iterator.
         *
         * This constructor initializes an AnyViewIterator with a specific iterator
         * instance, adapting it into the polymorphic AnyViewIterator interface. It
         * uses a template for flexibility, allowing various iterator types to be
         * passed, as long as they are convertible and suitable for the intended element type T.
         *
         * @tparam I The type of the input iterator.
         *
         * @param Iterator An iterator instance to initialize and adapt within this AnyViewIterator.
         */
        template <typename I>
            requires(!std::same_as<std::decay_t<I>, AnyViewIterator>) && std::same_as<std::iter_value_t<I>, T>
        constexpr explicit AnyViewIterator(I &&Iterator)
            : Delegate(std::in_place_type<AnyViewIteratorImpl<std::decay_t<I>>>, std::forward<I>(Iterator)) {
        }

        /**
         * @brief Compares two AnyViewIterator objects for equality.
         *
         * This operator checks if the current AnyViewIterator object is equal to another
         * AnyViewIterator object. Equality is determined by the underlying delegate's
         * `equal` function, which typically compares the internal state of the iterators
         * being pointed to by the AnyViewIterator instances.
         *
         * @param Other The other AnyViewIterator object to compare against.
         * @return true if both AnyViewIterator objects are equal, false otherwise.
         */
        constexpr bool operator==(const AnyViewIterator &Other) const {
            return Delegate->Equal(*Other.Delegate);
        }

        /**
         * @brief Compares the sentinel with another AnyViewSentinel object for equality.
         *
         * This operator checks if the current AnyViewSentinel object is equal to another
         * AnyViewSentinel object. Equality is determined by checking if the iteration
         * represented by delegate's iterator has reached the end of its associated view.
         *
         * @param Other The other AnyViewSentinel object to compare against.
         * @return true if the current iterator has reached the end of the view, false otherwise.
         */
        constexpr bool operator==(const AnyViewSentinel &Other) const {
            return Other.View->AtEnd(Delegate->Iterator());
        }

        /**
         * @brief Dereference operator for AnyViewIterator.
         *
         * This operator provides access to the element currently pointed to by the iterator.
         * It returns the element of type T, as retrieved from the iterator's underlying view
         * or collection.
         *
         * @return The element of type T currently pointed to by the iterator.
         */
        constexpr T operator*() const {
            return Delegate->Read();
        }

        /**
         * @brief Advances the AnyViewIterator to the next element in the sequence.
         *
         * This pre-increment operator moves the iterator to the next element
         * in its associated view or data source. The increment is performed by
         * delegating the operation to the underlying iterator implementation
         * encapsulated by the 'delegate' member. After calling this operator,
         * the iterator points to the subsequent element in its range.
         *
         * @return A reference to the incremented AnyViewIterator instance.
         */
        constexpr AnyViewIterator &operator++() {
            Delegate->Next();
            return *this;
        }

        /**
         * @brief Post-increment operator for AnyViewIterator.
         *
         * This operator advances the iterator to the next element in its sequence
         * and returns a copy of the iterator as it was before the increment.
         * This is useful in contexts where the current iterator position is
         * needed before moving to the next element.
         *
         * @return A copy of the iterator's state prior to the increment.
         */
        constexpr AnyViewIterator operator++(int) {
            auto Temp = *this;
            ++*this;
            return Temp;
        }

      private:
        Polymorphic<AnyViewIteratorInterface<T>> Delegate = AnyViewIteratorImpl<T *>(nullptr);
    };

    /**
     * @class AnyViewInterface
     *
     * @brief Template class that serves as an abstract interface for creating views over a collection,
     * providing type-erased iterators.
     *
     * The `AnyViewInterface` class is designed to allow derived classes to define their own view types,
     * enabling iteration over collections or sequences with type-erased iterator mechanics. By inheriting
     * from `ErasedView`, it ensures that derived view implementations provide functionality for both
     * beginning iteration and determining the end condition of the sequence through the `atEnd` method
     * inherited from `ErasedView`.
     *
     * @tparam T The type of elements that the view will iterate over.
     *
     * Key Responsibilities:
     * - Enforces the implementation of a `begin` method that provides an `AnyViewIterator` to start
     *   iterating over a collection or sequence of elements of type `T`.
     * - Supports polymorphic iteration start-point specification. Derived classes need to define their
     *   own mechanism for retrieving the starting iterator.
     */
    template <typename T>
    class AnyViewInterface : public ErasedView {
      public:
        /**
         * @brief Destructor for AnyViewInterface.
         *
         * The destructor for the `AnyViewInterface` class is declared as virtual
         * to ensure derived classes invoke the correct destructor when objects
         * are deleted through a pointer to this abstract interface.
         */
        virtual ~AnyViewInterface() = default;

        /**
         * @brief Pure virtual function to obtain the beginning iterator of the view.
         *
         * This function must be implemented by any class derived from `AnyViewInterface`.
         * It returns an `AnyViewIterator` that points to the first element of the collection
         * or sequence being viewed. This allows iteration over the elements of type `T`.
         *
         * @return An iterator of type `AnyViewIterator<T>` referring to the start of the view.
         */
        virtual AnyViewIterator<T> begin() = 0;
    };

    /**
     * @class AnyViewImpl
     *
     * @brief Template class providing an implementation of the AnyViewInterface for range-based views.
     *
     * This class serves as a concrete implementation of the AnyViewInterface, allowing for type-erased
     * iteration over a given range. It encapsulates a range of type `R` and provides interfaces to
     * begin iteration and to check the termination condition of the range.
     *
     * @tparam R The type of the range being viewed.
     * @tparam T The type of elements within the range. Defaults to the value type of the range.
     *
     * This implementation:
     * - Provides a constructor that accepts a range and ensures type safety through constraints.
     * - Implements the `begin` function to return an AnyViewIterator over the beginning of the range.
     * - Implements the `atEnd` function to check if the iterator has reached the end of the range.
     */
    template <typename R, typename T = std::ranges::range_value_t<R>>
    class AnyViewImpl : public AnyViewInterface<T> {
      public:
        /**
         * @brief Constructs an AnyViewImpl object with the given range.
         *
         * This constructor initializes the AnyViewImpl with a range by forwarding the provided
         * range argument. The use of std::forward allows for perfect forwarding of the range,
         * maintaining its value category (lvalue or rvalue) as originally passed.
         *
         * @tparam E The type of the range being forwarded.
         * @param Range The range to be encapsulated by the AnyViewImpl instance.
         *
         * @note This constructor is constrained to ensure that the type E is neither the
         * value type T nor any other non-decayable form, while also matching the decay type
         * of R, to ensure type safety and correctness.
         */
        template <typename E>
            requires(!std::same_as<std::decay_t<E>, T>) && std::same_as<std::decay_t<E>, R>
        constexpr explicit AnyViewImpl(E &&Range) : Range(std::forward<E>(Range)) {
        }

        /**
         * @brief Returns an iterator to the beginning of the encapsulated range.
         *
         * This function overrides the interface method to provide a concrete implementation
         * that initializes and returns an `AnyViewIterator` at the start of the encapsulated
         * range. It utilizes the `std::ranges::begin` function to obtain the starting point
         * of the range, enabling polymorphic iteration through the type-erased iterator.
         *
         * @return An `AnyViewIterator` object that points to the first element of the range.
         */
        AnyViewIterator<T> begin() override {
            return AnyViewIterator<T>(std::ranges::begin(Range));
        }

        /**
         * @brief Checks if the iterator has reached the end of the range.
         *
         * This function overrides the base class method to provide a specific
         * implementation for determining whether an iterator (retrieved from an
         * AnyRef object) has reached the end of the encapsulated range. It compares
         * the iterator against the range's end iterator, as provided by
         * `std::ranges::end`.
         *
         * @param Ref An `AnyRef` object containing the iterator to check.
         *
         * @return `true` if the iterator has reached the end of the range;
         * `false` otherwise.
         */
        bool AtEnd(AnyRef Ref) override {
            auto &It = Ref.get<const std::ranges::iterator_t<R>>();
            return It == std::ranges::end(Range);
        }

      private:
        R Range;
    };

    /**
     * @class AnyView
     *
     * @brief A type-erased view class supporting iteration over any range of input elements.
     *
     * AnyView is designed to abstract over various input ranges by providing a uniform
     * interface to interact with these ranges. It leverages type erasure to decouple the
     * range type from the client code, allowing seamless iteration and operations on
     * heterogeneous ranges. This class extends `std::ranges::view_interface` to facilitate
     * its integration into the C++ ranges framework.
     *
     * @tparam T The type of elements contained in the range this view will manage.
     *
     * The main functionalities provided by AnyView include:
     * - Construction from any range that is a valid input range, except from another AnyView.
     * - Assignment of new ranges to an existing AnyView, enabling dynamic range management.
     * - Iteration facilities through `begin` and `end` methods, which give access to
     *   AnyViewIterator and AnyViewSentinel, respectively.
     *
     * The class ensures type safety and efficient resource management using perfect forwarding
     * and SFINAE (Substitution Failure Is Not An Error) constraints, ensuring only valid
     * range types are accepted. Internally, it encapsulates the polymorphic behavior through
     * a `Polymorphic<AnyViewInterface<T>>` member, initializing with an empty view by default.
     */
    RETROLIB_EXPORT template <typename T>
    class AnyView : public std::ranges::view_interface<AnyView<T>> {

      public:
        /**
         * @brief Default constructor for AnyView.
         *
         * Constructs an empty AnyView object.
         */
        AnyView() = default;

        /**
         * @brief Constructs an AnyView object from a given range.
         *
         * This constructor initializes the `AnyView` with the provided range, encapsulating
         * it within the `AnyViewImpl` implementation. It uses perfect forwarding to accept
         * the range, allowing the `AnyView` to manage different types of ranges while
         * maintaining efficiency and type erasure.
         *
         * @tparam R The type of the range being passed; it must satisfy the input range
         *         requirements and must not be of type `AnyView` itself.
         *
         * @param Range The range object to encapsulate within the AnyView. The argument
         *              is forwarded to handle various value categories (lvalue or rvalue).
         *
         * @note This constructor is explicit with a boolean condition (usually `false`)
         *       to prevent implicit conversions and ensure intentional construction.
         */
        template <typename R>
            requires(!std::same_as<std::decay_t<R>, AnyView>) && std::ranges::input_range<R> && CompatibleRange<R, T>
        explicit(false) AnyView(R &&Range)
            : Data(std::in_place_type<AnyViewImpl<std::decay_t<R>>>, std::forward<R>(Range)) {
        }

        /**
         * @brief Assignment operator for assigning a range to an AnyView.
         *
         * This assignment operator allows an `AnyView` object to be assigned from a range.
         * The input range is encapsulated within the `AnyViewImpl` implementation to support
         * type erasure and maintain flexibility in range handling. The incoming range is
         * perfect-forwarded to ensure efficient handling of different types of ranges,
         * including lvalues and rvalues.
         *
         * @tparam R The type of the range being passed. It must satisfy the input range
         *           requirements and must not be of type `AnyView` itself.
         *
         * @param Range The range object to assign to the `AnyView`.
         *              The range is forwarded to accommodate different categories of value.
         *
         * @return A reference to the modified `AnyView` object, allowing for chaining
         *         assignments and further manipulations.
         */
        template <typename R>
            requires(!std::same_as<std::decay_t<R>, AnyView>) && std::ranges::input_range<R>
        AnyView &operator=(R &&Range) {
            Data = AnyViewImpl<std::decay_t<R>>(std::forward<R>(Range));
            return *this;
        }

        /**
         * @brief Returns the beginning iterator of the AnyView.
         *
         * This function provides access to an `AnyViewIterator` pointing to the first
         * element in the encapsulated range managed by `AnyView`. The actual type and
         * behavior of the iterator depend on the specific range implementation being used.
         *
         * @return An `AnyViewIterator` instance pointing to the initial element of the range.
         */
        AnyViewIterator<T> begin() {
            return Data->begin();
        }

        /**
         * @brief Returns the sentinel marking the end of the range in an AnyView.
         *
         * This member function provides access to an `AnyViewSentinel` object, which represents
         * the end of the range. It utilizes the underlying data of the `AnyView` object
         * to construct the sentinel, ensuring compatibility with the iteration process.
         *
         * @return An `AnyViewSentinel` that signals the end boundary of the range managed by `AnyView`.
         */
        AnyViewSentinel end() {
            return AnyViewSentinel(*Data);
        }

      private:
        Polymorphic<AnyViewInterface<T>> Data = AnyViewImpl<std::ranges::empty_view<T>>(std::views::empty<T>);
    };
} // namespace retro::ranges