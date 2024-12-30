/**
 * @file Generator.h
 * @brief Coroutine powered generator type.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if RETROLIB_WITH_COROUTINES

#if !RETROLIB_WITH_MODULES
#include "RetroLib/RetroLibMacros.h"

#include <bit>
#include <cassert>
#include <coroutine>
#include <exception>
#include <iterator>
#include <type_traits>
#include <utility>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {
    template <typename T>
    class ManualLifetime {
      public:
        ManualLifetime() noexcept {
            // We need to manually construct to avoid an attempted construction
        }

        ~ManualLifetime() {
            // We need this to get the union to destruct manually
        }

        template <typename... A>
        T &Construct(A &&...Args) noexcept(std::is_nothrow_constructible_v<T, A...>) {
            return *::new (static_cast<void *>(std::addressof(Value))) T(static_cast<A &&>(Args)...);
        }

        void Destruct() noexcept(std::is_nothrow_destructible_v<T>) {
            Value.~T();
        }

        T &Get() & noexcept {
            return Value;
        }
        T &&Get() && noexcept {
            return static_cast<T &&>(Value);
        }
        const T &Get() const & noexcept {
            return Value;
        }
        const T &&Get() const && noexcept {
            return static_cast<const T &&>(Value);
        }

      private:
        union {
            std::remove_const_t<T> Value;
        };
    };

    template <typename T>
    class ManualLifetime<T &> {
      public:
        T &Construct(T &ValueIn) noexcept {
            Value = std::addressof(ValueIn);
            return ValueIn;
        }

        void Destruct() noexcept {
            // No special destruction needed here
        }

        T &Get() const noexcept {
            return *Value;
        }

      private:
        T *Value = nullptr;
    };

    template <typename T>
    class ManualLifetime<T &&> {
      public:
        T &&Construct(T &&ValueIn) noexcept {
            Value = std::addressof(ValueIn);
            return static_cast<T &&>(ValueIn);
        }

        void Destruct() noexcept {
            // No special destruction needed here
        }

        T &&Get() const noexcept {
            return static_cast<T &&>(*Value);
        }

      private:
        T *Value = nullptr;
    };

    struct UseAllocatorArg {};

    namespace Ranges {

        /**
         * @brief Represents a collection or group of elements.
         *
         * The ElementsOf class is a utility that encapsulates a set of elements.
         * This class is used as a tag for overload resolution primarily.
         */
        RETROLIB_EXPORT template <typename R, typename A = UseAllocatorArg>
        struct ElementsOf {
            /**
             * @brief Constructor for the ElementsOf class.
             *
             * Initializes an ElementsOf object with the given range. This constructor
             * is marked explicit, constexpr, and noexcept, ensuring that it performs
             * minimal compile-time overhead and guarantees no exceptions. The constructor
             * also requires that the type A is default constructible.
             *
             * @tparam R The type of the range being passed to the constructor.
             * @param Range The range to be used for initialization.
             */
            explicit constexpr ElementsOf(R &&Range) noexcept
                requires std::is_default_constructible_v<A>
                : Range(static_cast<R &&>(Range)) {
            }

            /**
             * @brief Constructor for the ElementsOf class.
             *
             * Constructs an ElementsOf object, initializing it with the provided range and allocator.
             * This constructor is marked explicit, constexpr, and noexcept, ensuring it executes at
             * compile time when possible and does not throw exceptions. It processes the given parameters
             * using static casting to achieve perfect forwarding.
             *
             * @tparam R The type of the range being passed to the constructor.
             * @tparam A The type of the allocator being used.
             * @param Range The range to initialize the ElementsOf object with.
             * @param Alloc The allocator to use for initializing the object.
             */
            constexpr ElementsOf(R &&Range, A &&Alloc) noexcept
                : Alloc(static_cast<A &&>(Alloc)), Range(static_cast<R &&>(Range)) {
            }

            /**
             * @brief Copy constructor for the ElementsOf class (deleted).
             *
             * This constructor is explicitly deleted to prevent copying of ElementsOf objects.
             * The deletion ensures that the class adheres to the rule of five and avoids
             * unintentional copying, which could lead to resource management issues.
             *
             * @param other The ElementsOf object being copied (not allowed).
             */
            constexpr ElementsOf(const ElementsOf &) = delete;

            /**
             * @brief Move constructor for the ElementsOf class.
             *
             * This move constructor is marked as constexpr and noexcept, ensuring that it is
             * suitable for compile-time operations and guarantees no exceptions during execution.
             * Default implementation is used to perform a minimal move operation.
             *
             * @param other The ElementsOf instance to be moved from.
             */
            constexpr ElementsOf(ElementsOf &&) noexcept = default;

            /**
             * @brief Deleted copy assignment operator for the ElementsOf class.
             *
             * Prevents copying of an ElementsOf instance by deleting the copy assignment operator.
             *
             * @param other The ElementsOf instance to be copied from (unused).
             * @return A reference to the ElementsOf instance (not applicable as the method is deleted).
             */
            constexpr ElementsOf &operator=(const ElementsOf &) = delete;

            /**
             * @brief Deleted move assignment operator for the ElementsOf class.
             *
             * Prevents the move assignment operation for ElementsOf instances. Marked as `constexpr`
             * and `delete` to explicitly prohibit the use of this operator during compile time, ensuring
             * immutability or non-transferable semantics for this class.
             *
             * @return This function does not return anything as it is deleted.
             */
            constexpr ElementsOf &operator=(ElementsOf &&) = delete;

            /**
             * @brief Retrieves the stored range with guaranteed no exception.
             *
             * This function returns the stored range by casting it to an rvalue reference.
             * It ensures minimal overhead and adheres to noexcept specifications, guaranteeing
             * that no exceptions will be thrown during its execution.
             *
             * @return R&& An rvalue reference to the stored range.
             */
            constexpr R &&Get() noexcept {
                return static_cast<R &&>(Range);
            }

            /**
             * @brief Retrieves the allocator associated with the instance.
             *
             * This method returns the allocator of type A used by the instance. It is marked as constexpr
             * and noexcept, ensuring minimal runtime overhead and guaranteeing no exceptions will be thrown.
             *
             * @return The allocator object associated with the instance.
             */
            constexpr A GetAllocator() const noexcept {
                return Alloc;
            }

          private:
            [[no_unique_address]] A Alloc; // \expos
            R &&Range;                     // \expos
        };

        template <typename R>
        ElementsOf(R &&) -> ElementsOf<R>;

        template <typename R, typename A>
        ElementsOf(R &&, A &&) -> ElementsOf<R, A>;

    } // namespace ranges

    template <typename A>
    static constexpr bool AllocatorNeedsToBeStored =
        !std::allocator_traits<A>::is_always_equal::value || !std::is_default_constructible_v<A>;

    // Round s up to next multiple of a.
    constexpr size_t AlignedAllocationSize(size_t s, size_t a) {
        return (s + a - 1) & ~(a - 1);
    }

    RETROLIB_EXPORT template <typename R, typename V = std::remove_cvref_t<R>, typename A = UseAllocatorArg>
    class Generator;

    template <typename A>
    class PromiseBaseAlloc {
        static constexpr std::size_t OffsetOfAllocator(std::size_t FrameSize) noexcept {
            return AlignedAllocationSize(FrameSize, alignof(A));
        }

        static constexpr std::size_t PaddedFrameSize(std::size_t FrameSize) noexcept {
            return OffsetOfAllocator(FrameSize) + sizeof(A);
        }

        static A &GetAllocator(void *Frame, std::size_t FrameSize) noexcept {
            return *std::bit_cast<A *>(static_cast<char *>(Frame) + OffsetOfAllocator(FrameSize));
        }

      public:
        template <typename... T>
        void *operator new(std::size_t FrameSize, std::allocator_arg_t, A Alloc, T &...) {
            void *Frame = Alloc.allocate(PaddedFrameSize(FrameSize));

            // Store allocator at end of the coroutine frame.
            // Assuming the allocator's move constructor is non-throwing (a requirement for allocators)
            ::new (static_cast<void *>(std::addressof(GetAllocator(Frame, FrameSize)))) A(std::move(Alloc));

            return Frame;
        }

        template <typename T, typename... U>
        void *operator new(std::size_t FrameSize, T &, std::allocator_arg_t, A Alloc, U &...) {
            return PromiseBaseAlloc::operator new(FrameSize, std::allocator_arg, std::move(Alloc));
        }

        void operator delete(void *Ptr, std::size_t FrameSize) noexcept {
            A &alloc = GetAllocator(Ptr, FrameSize);
            A local_alloc(std::move(alloc));
            alloc.~A();
            local_alloc.deallocate(static_cast<std::byte *>(Ptr), PaddedFrameSize(FrameSize));
        }
    };

    template <typename A>
        requires(!AllocatorNeedsToBeStored<A>)
    class PromiseBaseAlloc<A> {
      public:
        void *operator new(std::size_t Size) {
            A Alloc;
            return Alloc.allocate(Size);
        }

        void operator delete(void *Ptr, std::size_t Size) noexcept {
            A Alloc;
            Alloc.deallocate(static_cast<std::byte *>(Ptr), Size);
        }
    };

    template <typename T>
    struct GeneratorPromiseBase {
        template <typename R, typename V, typename A>
        friend class Generator;

        GeneratorPromiseBase *Root;
        std::coroutine_handle<> ParentOrLeaf;
        // Note: Using manual_lifetime here to avoid extra calls to exception_ptr
        // constructor/destructor in cases where it is not needed (i.e. where this
        // generator coroutine is not used as a nested coroutine).
        // This member is lazily constructed by the __yield_sequence_awaiter::await_suspend()
        // method if this generator is used as a nested generator.
        ManualLifetime<std::exception_ptr> Exception;
        ManualLifetime<T> Value;

        explicit GeneratorPromiseBase(std::coroutine_handle<> ThisCoro) noexcept
            : Root(this), ParentOrLeaf(ThisCoro) {
        }

        ~GeneratorPromiseBase() {
            if (Root != this) {
                // This coroutine was used as a nested generator and so will
                // have constructed its __exception_ member which needs to be
                // destroyed here.
                Exception.Destruct();
            }
        }

        std::suspend_always initial_suspend() noexcept {
            return {};
        }

        void return_void() noexcept {
        }

        void unhandled_exception() {
            if (Root != this) {
                Exception.Get() = std::current_exception();
            } else {
                throw;
            }
        }

        // Transfers control back to the parent of a nested coroutine
        struct FinalAwaiter {
            bool await_ready() noexcept {
                return false;
            }

            template <typename P>
            std::coroutine_handle<> await_suspend(std::coroutine_handle<P> Handle) noexcept {
                auto &Promise = Handle.promise();
                auto &CurrentRoot = *Promise.Root;
                if (&CurrentRoot != &Promise) {
                    auto Parent = Promise.ParentOrLeaf;
                    CurrentRoot.ParentOrLeaf = Parent;
                    return Parent;
                }
                return std::noop_coroutine();
            }

            void await_resume() noexcept {
                // No implementation needed here
            }
        };

        FinalAwaiter final_suspend() noexcept {
            return {};
        }

        std::suspend_always yield_value(T &&x) noexcept(std::is_nothrow_move_constructible_v<T>) {
            Root->Value.Construct(std::move(x));
            return {};
        }

        template <typename V>
            requires(!std::is_reference_v<T>) && std::is_convertible_v<V, T>
        std::suspend_always yield_value(V &&x) noexcept(std::is_nothrow_constructible_v<T, V>) {
            Root->Value.Construct(std::forward<V>(x));
            return {};
        }

        template <typename G>
        struct YieldSequenceAwaiter {
            G Gen;

            explicit(false) YieldSequenceAwaiter(G &&Gen) noexcept
                // Taking ownership of the generator ensures frame are destroyed
                // in the reverse order of their execution.
                : Gen(static_cast<G &&>(Gen)) {
            }

            bool await_ready() noexcept {
                return false;
            }

            // set the parent, root and exceptions pointer and
            // resume the nested
            template <typename Promise>
            std::coroutine_handle<> await_suspend(std::coroutine_handle<Promise> Handle) noexcept {
                auto &Current = Handle.promise();
                auto &Nested = *Gen.GetPromise();
                auto &CurrentRoot = *Current.Root;

                Nested.Root = Current.Root;
                Nested.ParentOrLeaf = Handle;

                // Lazily construct the __exception_ member here now that we
                // know it will be used as a nested generator. This will be
                // destroyed by the promise destructor.
                Nested.Exception.Construct();
                CurrentRoot.ParentOrLeaf = Gen.GetCoro();

                // Immediately resume the nested coroutine (nested generator)
                return Gen.GetCoro();
            }

            void await_resume() {
                if (auto &NestedPromise = *Gen.GetPromise(); NestedPromise.Exception.Get()) {
                    std::rethrow_exception(std::move(NestedPromise.Exception.Get()));
                }
            }
        };

        template <typename O, typename A>
        YieldSequenceAwaiter<Generator<T, O, A>>
        yield_value(Ranges::ElementsOf<Generator<T, O, A>> Generator) noexcept {
            return std::move(Generator).Get();
        }

        template <std::ranges::range R, typename A>
        YieldSequenceAwaiter<Generator<T, std::remove_cvref_t<T>, A>> yield_value(Ranges::ElementsOf<R, A> &&X) {
            return [](std::allocator_arg_t, A, auto &&Range) -> Generator<T, std::remove_cvref_t<T>, A> {
                for (auto &&E : Range)
                    co_yield static_cast<decltype(E)>(E);
            }(std::allocator_arg, X.get_allocator(), std::forward<R>(X.get()));
        }

        void resume() {
            ParentOrLeaf.resume();
        }

        // Disable use of co_await within this coroutine.
        void await_transform() = delete;
    };

    template <typename G, typename B, bool ExplicitAllocator = false>
    struct GeneratorPromise;

    template <typename T, typename V, typename A, typename B, bool ExplicitAllocator>
    struct GeneratorPromise<Generator<T, V, A>, B, ExplicitAllocator> final : public GeneratorPromiseBase<T>,
                                                                              public PromiseBaseAlloc<B> {
        GeneratorPromise() noexcept
            : GeneratorPromiseBase<T>(std::coroutine_handle<GeneratorPromise>::from_promise(*this)) {
        }

        Generator<T, V, A> get_return_object() noexcept {
            return Generator<T, V, A>{std::coroutine_handle<GeneratorPromise>::from_promise(*this)};
        }

        using GeneratorPromiseBase<T>::yield_value;

        template <std::ranges::range R>
        typename GeneratorPromiseBase<T>::template YieldSequenceAwaiter<Generator<T, V, A>>
        yield_value(Ranges::ElementsOf<R> &&X) {
            static_assert(!ExplicitAllocator, "This coroutine has an explicit allocator specified with "
                                              "std::allocator_arg so an allocator needs to be passed "
                                              "explicitly to std::elements_of");
            return []<typename U> (U &&Range) -> Generator<T, V, A> {
                for (auto &&E : std::forward<U>(Range))
                    co_yield std::forward<decltype(E)>(E);
            }(std::forward<R>(X.get()));
        }
    };

    template <typename A>
    using ByteAllocatorType = typename std::allocator_traits<std::remove_cvref_t<A>>::template rebind_alloc<std::byte>;
} // namespace retro

namespace std {
    // Type-erased allocator with default allocator behaviour.
    RETROLIB_EXPORT template <typename R, typename V, typename... A>
    struct coroutine_traits<Retro::Generator<R, V>, A...> {
        using promise_type = Retro::GeneratorPromise<Retro::Generator<R, V>, std::allocator<std::byte>>;
    };

    // Type-erased allocator with std::allocator_arg parameter
    RETROLIB_EXPORT template <typename R, typename V, typename A, typename... T>
    struct coroutine_traits<Retro::Generator<R, V>, allocator_arg_t, A, T...> {
      private:
        using ByteAllocator = Retro::ByteAllocatorType<A>;

      public:
        using promise_type =
            Retro::GeneratorPromise<Retro::Generator<R, V>, ByteAllocator, true /*explicit Allocator*/>;
    };

    // Type-erased allocator with std::allocator_arg parameter (non-static member functions)
    RETROLIB_EXPORT template <typename R, typename V, typename S, typename A, typename... T>
    struct coroutine_traits<Retro::Generator<R, V>, S, allocator_arg_t, A, T...> {
      private:
        using ByteAllocator = Retro::ByteAllocatorType<A>;

      public:
        using promise_type =
            Retro::GeneratorPromise<Retro::Generator<R, V>, ByteAllocator, true /*explicit Allocator*/>;
    };

    // Generator with specified allocator type
    RETROLIB_EXPORT template <typename R, typename V, typename A, typename... T>
    struct coroutine_traits<Retro::Generator<R, V, A>, T...> {
        using ByteAllocator = Retro::ByteAllocatorType<A>;

      public:
        using promise_type = Retro::GeneratorPromise<Retro::Generator<R, V, A>, ByteAllocator>;
    };
} // namespace std

namespace Retro {
    /**
     * @class Generator
     * @brief The Generator class is a type of coroutine powered view. Allowing the user to define a method that
     * executes until a `co_yield` statement and then stalling until the next iteration of the loop.
     *
     * @tparam R The reference type of the generator
     * @tparam V The value type of the generator (defaults to the cvref unqualified version of R)
     * @tparam A The allocator used for this type
     */
    template <typename R, typename V, typename A>
    class Generator : public std::ranges::view_interface<Generator<R, V, A>> {
        using ByteAllocator = ByteAllocatorType<A>;

      public:
        using promise_type = GeneratorPromise<Generator, ByteAllocator>;
        friend promise_type;

      private:
        using CoroutineHandle = std::coroutine_handle<promise_type>;

    public:
        /**
         * @brief Default constructor for the Generator class.
         *
         * This constructor initializes a Generator object. It is marked noexcept,
         * indicating that it guarantees not to throw any exceptions.
         */
        Generator() noexcept = default;

        /**
         * @brief Move constructor for the Generator class.
         *
         * Constructs a new Generator object by transferring ownership of resources
         * from another Generator instance. The source Generator is left in a valid,
         * but unspecified state. This constructor is marked noexcept, ensuring it
         * does not throw exceptions.
         *
         * @param Other Another Generator instance to move from.
         *              The ownership of its resources will be transferred.
         * @return A newly constructed Generator object with the transferred resources.
         */
        Generator(Generator &&Other) noexcept
            : Coroutine(std::exchange(Other.coroutine, {})), Started(std::exchange(Other.started, false)) {
        }

        /**
         * @brief Destructor for the Generator class.
         *
         * Cleans up resources used by the Generator object. If a coroutine exists
         * and has started but is not completed, the destructor ensures proper
         * destruction of the coroutine's value before destroying the coroutine.
         * It is marked noexcept, guaranteeing that no exceptions will be thrown
         * during resource cleanup.
         */
        ~Generator() noexcept {
            if (Coroutine) {
                if (Started && !Coroutine.done()) {
                    Coroutine.promise().value.destruct();
                }
                Coroutine.destroy();
            }
        }

        /**
         * @brief Move assignment operator for the Generator class.
         *
         * Assigns the state of another Generator instance to this one using move semantics.
         * This method guarantees not to throw any exceptions and swaps the contents of the
         * two instances to facilitate the move.
         *
         * @param g A reference to the Generator instance to be moved.
         * @return A reference to the current Generator instance.
         */
        Generator &operator=(Generator &&g) noexcept {
            swap(g);
            return *this;
        }

        /**
         * @brief Exchanges the state of this Generator object with another.
         *
         * This method swaps the internal state, including the coroutine and
         * the started state, between this Generator instance and the provided one.
         * It is marked noexcept, ensuring that no exceptions are thrown during the operation.
         *
         * @param other The Generator object to swap with.
         */
        void swap(Generator &other) noexcept {
            std::swap(Coroutine, other.coroutine);
            std::swap(Started, other.started);
        }

    private:
        struct Sentinel {};

        class Iterator {
          public:
            using iterator_category = std::input_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = V;
            using reference = R;
            using pointer = std::add_pointer_t<R>;

            Iterator() noexcept = default;
            Iterator(const Iterator &) = delete;

            Iterator(Iterator &&Other) noexcept : Coroutine(std::exchange(Other.Coroutine, {})) {
            }

            Iterator &operator=(const Iterator &Other) = delete;

            Iterator &operator=(Iterator &&Other) noexcept {
                std::swap(Coroutine, Other.Coroutine);
                return *this;
            }

            ~Iterator() = default;

            friend bool operator==(const Iterator &It, Sentinel) noexcept {
                return It.Coroutine.done();
            }

            Iterator &operator++() {
                Coroutine.promise().value.destruct();
                Coroutine.promise().resume();
                return *this;
            }
            void operator++(int) {
                (void)operator++();
            }

            reference operator*() const noexcept {
                return static_cast<reference>(Coroutine.promise().Value.Get());
            }

          private:
            friend Generator;

            explicit Iterator(CoroutineHandle Coroutine) noexcept : Coroutine(Coroutine) {
            }

            CoroutineHandle Coroutine;
        };

    public:
        /**
         * @brief Returns an iterator pointing to the beginning of the sequence.
         *
         * This method initializes the coroutine if it has not already started, resumes
         * it, and returns an iterator at the beginning of the sequence.
         *
         * @return An iterator pointing to the beginning of the sequence.
         */
        Iterator begin() {
            RETROLIB_ASSERT(Coroutine);
            RETROLIB_ASSERT(!Started);
            Started = true;
            Coroutine.resume();
            return Iterator{Coroutine};
        }

        /**
         * @brief Retrieves the end sentinel of a range.
         *
         * This function returns a Sentinel object representing the end of a range.
         * It is marked noexcept, ensuring that it does not throw any exceptions.
         */
        Sentinel end() noexcept {
            return {};
        }

      private:
        explicit Generator(CoroutineHandle coroutine) noexcept : Coroutine(coroutine) {
        }

    public:
        /**
         * @brief Retrieves the current coroutine handle.
         *
         * This function returns the handle to the coroutine associated with this object.
         * It is marked noexcept, indicating that it guarantees not to throw any exceptions.
         *
         * @return A coroutine handle representing the current coroutine.
         */
        std::coroutine_handle<> GetCoro() noexcept {
            return Coroutine;
        }

        /**
         * @brief Retrieves the promise associated with the coroutine.
         *
         * This function returns a pointer to the promise object of the coroutine.
         * It is noexcept, ensuring it does not throw exceptions.
         *
         * @return A pointer to the promise_type object.
         */
        promise_type *GetPromise() noexcept {
            return std::addressof(Coroutine.promise());
        }

      private:
        CoroutineHandle Coroutine;
        bool Started = false;
    };

    /**
     * @class Generator
     * @brief The Generator class is a type of coroutine powered view. Allowing the user to define a method that
     * executes until a `co_yield` statement and then stalling until the next iteration of the loop.
     *
     * @tparam R The reference type of the generator
     * @tparam V The value type of the generator (defaults to the cvref unqualified version of R)
     */
    template <typename R, typename V>
    class Generator<R, V, UseAllocatorArg> : public std::ranges::view_interface<Generator<R, V>> {
        using PromiseBase = GeneratorPromiseBase<R>;

    public:
        /**
         * @brief Default constructor for the Generator class.
         *
         * Constructs a Generator instance. This constructor is explicitly marked as noexcept,
         * ensuring it does not throw any exceptions during its execution.
         */
        Generator() noexcept = default;

        /**
         * @brief Move constructor for the Generator class.
         *
         * Transfers ownership of resources from another Generator instance to this instance.
         * Marks the operation as noexcept, guaranteeing no exceptions will be thrown.
         * It exchanges the internal state with the provided Generator object, leaving the
         * other object in a valid but unspecified state.
         *
         * @param Other The Generator instance to move resources from.
         */
        Generator(Generator &&Other) noexcept
            : Promise(std::exchange(Other.Promise, nullptr)), Coroutine(std::exchange(Other.Coroutine, {})),
              Started(std::exchange(Other.Started, false)) {
        }

        /**
         * @brief Destructor for the Generator class.
         *
         * Cleans up the Generator object by ensuring proper destruction of
         * resources. If the coroutine exists, it first checks if the
         * coroutine was started and not yet completed, in which case it
         * destructs the stored value. Finally, the coroutine is destroyed.
         * This destructor is marked noexcept, guaranteeing that it does not
         * throw any exceptions during its execution.
         */
        ~Generator() noexcept {
            if (Coroutine) {
                if (Started && !Coroutine.done()) {
                    Promise->Value.Destruct();
                }
                Coroutine.destroy();
            }
        }

        /**
         * @brief Assignment operator for the Generator class.
         *
         * Performs copy-and-swap idiom to assign the contents of the given Generator
         * instance to the current one. This operator is marked noexcept, indicating
         * that it guarantees not to throw any exceptions.
         *
         * @param g The Generator object to be assigned.
         * @return A reference to the updated Generator object.
         */
        Generator &operator=(Generator g) noexcept {
            swap(g);
            return *this;
        }

        /**
         * @brief Exchanges the contents of this Generator with another.
         *
         * This method swaps the internal state of the Generator with the provided one,
         * ensuring that their resources, such as promises and coroutines, are exchanged.
         * It is marked noexcept, guaranteeing no exceptions will be thrown during the swap.
         *
         * @param Other The Generator instance to swap with.
         */
        void swap(Generator &Other) noexcept {
            std::swap(Promise, Other.promise);
            std::swap(Coroutine, Other.coroutine);
            std::swap(Started, Other.started);
        }

    private:
        struct Sentinel {};

        class Iterator {
          public:
            using iterator_category = std::input_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = V;
            using reference = R;
            using pointer = std::add_pointer_t<R>;

            Iterator() noexcept = default;
            Iterator(const Iterator &) = delete;

            Iterator(Iterator &&Other) noexcept
                : Promise(std::exchange(Other.Promise, nullptr)), Coroutine(std::exchange(Other.Coroutine, {})) {
            }

            Iterator &operator=(const Iterator &) = default;

            Iterator &operator=(Iterator &&Other) noexcept {
                Promise = std::exchange(Other.Promise, nullptr);
                Coroutine = std::exchange(Other.Coroutine, {});
                return *this;
            }

            ~Iterator() = default;

            friend bool operator==(const Iterator &It, Sentinel) noexcept {
                return It.Coroutine.done();
            }

            Iterator &operator++() {
                Promise->Value.Destruct();
                Promise->resume();
                return *this;
            }

            void operator++(int) {
                (void)operator++();
            }

            reference operator*() const noexcept {
                return static_cast<reference>(Promise->Value.Get());
            }

          private:
            friend Generator;

            explicit Iterator(PromiseBase *Promise, std::coroutine_handle<> Coroutine) noexcept
                : Promise(Promise), Coroutine(Coroutine) {
            }

            PromiseBase *Promise;
            std::coroutine_handle<> Coroutine;
        };

    public:
        /**
         * @brief Returns an iterator representing the beginning of a range.
         *
         * This method initializes the coroutine if it hasn't already been started,
         * validates the coroutine state, and resumes execution to prepare the range
         * for iteration. An iterator is then returned to allow traversal.
         *
         * @return Iterator pointing to the start of the range.
         *
         * @note The coroutine must be valid and not have started prior to this call.
         */
        Iterator begin() {
            RETROLIB_ASSERT(Coroutine);
            RETROLIB_ASSERT(!Started);
            Started = true;
            Coroutine.resume();
            return Iterator{Promise, Coroutine};
        }

        /**
         * @brief Returns a sentinel indicating the end of a range.
         *
         * This method provides a sentinel object that represents the end
         * of a range. It is noexcept, ensuring that it does not throw any exceptions.
         *
         * @return A Sentinel object representing the end of the range.
         */
        Sentinel end() noexcept {
            return {};
        }

      private:
        template <typename G, typename B, bool ExplicitAllocator>
        friend struct GeneratorPromise;

        template <typename P>
        explicit Generator(std::coroutine_handle<P> Coroutine) noexcept
            : Promise(std::addressof(Coroutine.promise())), Coroutine(Coroutine) {
        }

    public:
        /**
        * @brief Retrieves the coroutine handle.
        *
        * This method provides access to the internal coroutine handle. It is marked noexcept,
        * indicating that it guarantees not to throw any exceptions.
        *
        * @return The coroutine handle associated with this object.
        */
        std::coroutine_handle<> GetCoro() noexcept {
            return Coroutine;
        }

        /**
         * @brief Retrieves the associated promise object.
         *
         * This function returns a pointer to the promise object associated
         * with this instance. It is marked noexcept, ensuring it does not
         * throw exceptions.
         *
         * @return Pointer to the associated promise object.
         */
        PromiseBase *GetPromise() noexcept {
            return Promise;
        }

      private:
        PromiseBase *Promise = nullptr;
        std::coroutine_handle<> Coroutine;
        bool Started = false;
    };
} // namespace retro

#endif