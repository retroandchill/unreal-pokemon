/**
 * @file Polymorphic.h
 * @brief Contains the declaration for the polymorphic class.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Concepts/Inheritance.h"
#include "RetroLib/Concepts/OpaqueStorage.h"

#include <array>
#include <typeinfo>
#include <utility>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {
    /**
     * @brief A class template that provides polymorphic storage and access capabilities for types derived from a base
     * class.
     *
     * The Polymorphic class allows storing objects of different derived types within an instance without losing the
     * polymorphic behavior. It provides interfaces to manage the lifetime and access the stored object polymorphically.
     *
     * @tparam T The base class type which all stored objects must derive from. Must satisfy the ClassType concept.
     */
    RETROLIB_EXPORT template <ClassType T, size_t SmallStorageSize = DEFAULT_SMALL_STORAGE_SIZE>
        requires(SmallStorageSize >= sizeof(void *))
    class Polymorphic {
        template <typename U>
            requires std::derived_from<U, T>
        static constexpr bool fitsSmallStorage = sizeof(U) <= SmallStorageSize;

      public:
        /**
         * Default constructor for the Polymorphic class.
         *
         * This constructor initializes the storage for the Polymorphic object
         * by emplacing an instance of type T. It is only available if T is
         * default constructible.
         *
         * @tparam T The polymorphic type that this class is managing.
         *
         * @pre std::is_default_constructible_v<T> must be true, meaning
         * the type T must have a default constructor.
         *
         * @throws No exceptions are thrown. The constructor is marked noexcept.
         */
        constexpr Polymorphic() noexcept
            requires std::is_default_constructible_v<T>
        {
            storage.template emplace<T>();
        }

        /**
         * Constructs a Polymorphic object from a value of type U.
         *
         * This constructor initializes the Polymorphic object using the supplied value,
         * and assigns the appropriate vtable for type U, ensuring polymorphic behavior.
         *
         * @tparam U The type of the value used to construct the Polymorphic object.
         *          It must be a type derived from T.
         *
         * @param value An instance of type U, which will be stored within the Polymorphic object.
         *
         * @pre U must be derived from T.
         *
         * @post The Polymorphic object is initialized with the given value and the
         *       vtable for type U is set.
         *
         * @throws No exceptions are thrown as the constructor is marked noexcept.
         */
        template <typename U>
            requires std::derived_from<std::decay_t<U>, T>
        explicit(false) constexpr Polymorphic(U &&value) noexcept : vtable(getVTable<U>()) {
            storage.template emplace<std::decay_t<U>>(std::forward<U>(value));
        }

        /**
         * Constructs a Polymorphic object with an in-place type and arguments.
         *
         * This constructor initializes the Polymorphic object by directly constructing
         * an instance of type U within the storage using the provided arguments. It is
         * only available for types U that are derived from T and constructible from
         * the provided argument types A.
         *
         * @tparam U The type of object to be constructed within the Polymorphic. It must
         *        be a type derived from the base type T.
         * @tparam A Parameter pack representing the types of the arguments used for
         *        constructing the U type object.
         *
         * @param args Arguments to be forwarded to the constructor of U.
         *
         * @pre U must be derived from T and must be constructible from the arguments of types A.
         *
         * @post The Polymorphic object is initialized with an instance of U and the
         *       corresponding vtable for handling polymorphic operations is set.
         *
         * @throws No exceptions are thrown as the constructor is marked noexcept.
         */
        template <typename U, typename... A>
            requires std::derived_from<U, T> && std::constructible_from<U, A...>
        explicit constexpr Polymorphic(std::in_place_type_t<U>, A &&...args) noexcept : vtable(getVTable<U>()) {
            storage.template emplace<U>(std::forward<A>(args)...);
        }

        /**
         * @brief Copy constructor for the Polymorphic class.
         *
         * This constructor initializes a new Polymorphic object as a copy of an existing one.
         * It copies the vtable pointer from the source object and uses the vtable to copy
         * the underlying storage, ensuring that the copied object retains polymorphic behavior
         * identical to the source object.
         *
         * @param other The Polymorphic object to be copied.
         *
         * @post A new Polymorphic object is created with the same polymorphic type and state
         *       as the object provided.
         *
         * @throws No exceptions are thrown. The constructor is marked noexcept.
         */
        constexpr Polymorphic(const Polymorphic &other) noexcept : vtable(other.vtable) {
            vtable->copy(other.storage, storage);
        }

        /**
         * @brief Move constructor for the Polymorphic class.
         *
         * Constructs a new Polymorphic object by transferring ownership of the
         * resources from an existing Polymorphic object. The vtable is copied from
         * the source object, and the associated move operation defined in the
         * vtable is performed on the source's storage, transferring its contents
         * to the new object's storage.
         *
         * @param other The Polymorphic object to be moved from. After the move,
         *              the source object is left in a valid but unspecified state.
         *
         * @post The current object is initialized with the transferred state
         *       from `other`, and the vtable pointer is set appropriately.
         *
         * @throws No exceptions are thrown. The constructor is marked noexcept.
         */
        constexpr Polymorphic(Polymorphic &&other) noexcept : vtable(other.vtable) {
            vtable->move(other.storage, storage);
        }

        /**
         * @brief Destructor for the Polymorphic class.
         *
         * This destructor is responsible for properly destroying the dynamically-typed
         * object managed by the Polymorphic instance. It accesses the appropriate destructor
         * function through the vtable and applies it to the internal storage, ensuring
         * that the object is cleaned up correctly according to its actual runtime type.
         *
         * @note This operation is marked as noexcept, indicating that no exceptions will be
         *       thrown during the destruction process.
         */
        constexpr ~Polymorphic() noexcept {
            vtable->destroy(storage);
        }

        /**
         * Assigns the contents of another Polymorphic object to this one.
         *
         * This assignment operator checks whether the dynamic types of the two objects are the same.
         * If they are, it assigns the values by invoking a copy assignment operation through the vtable.
         * If the types differ, it destroys the current contents and performs a new copy via the vtable.
         *
         * @param other The Polymorphic object to be assigned to the current object.
         * @return A reference to the current object after assignment.
         * @note The operation is noexcept, implying it does not throw exceptions.
         */
        constexpr Polymorphic &operator=(const Polymorphic &other) noexcept {
            if (vtable->getType() == other.vtable->getType()) {
                vtable = other.vtable;
                vtable->copyAssign(other.storage, storage);
            } else {
                vtable->destroy(storage);
                vtable = other.vtable;
                vtable->copy(other.storage, storage);
            }

            return *this;
        }

        /**
         * Move assignment operator for the Polymorphic class.
         *
         * This operator allows the transfer of resources from another Polymorphic instance
         * to the current instance, supporting polymorphic behavior through a vtable mechanism.
         * It checks if the current object's type, as defined by its vtable, matches the type
         * of the other object. If so, it directly assigns the vtable and moves the resources.
         * If not, it destroys the current resources, assigns the new vtable, and moves the resources
         * from the other object.
         *
         * @param other A rvalue reference to the Polymorphic instance being assigned from.
         *
         * @return A reference to the current instance after assignment.
         *
         * @note The operation is noexcept, ensuring that it does not throw exceptions.
         */
        constexpr Polymorphic &operator=(Polymorphic &&other) noexcept {
            if (vtable->getType() == other.vtable->getType()) {
                vtable = other.vtable;
                vtable->moveAssign(other.storage, storage);
            } else {
                vtable->destroy(storage);
                vtable = other.vtable;
                vtable->move(other.storage, storage);
            }
            return *this;
        }

        /**
         * Assigns a value of a type derived from T to the polymorphic object.
         *
         * This operator allows assigning an object of a type that is derived from the base type T
         * to this polymorphic object. The assignment is only available if U is a derived type
         * of T, as constrained by the std::derived_from requirement.
         *
         * @tparam U A type that is derived from T.
         * @param value An rvalue reference to the object of type U that is to be assigned.
         * @return A reference to the updated polymorphic object.
         *
         * @note The operation is noexcept, meaning it guarantees not to throw exceptions.
         */
        template <typename U>
            requires std::derived_from<std::decay_t<U>, T>
        constexpr Polymorphic &operator=(U &&value) noexcept {
            emplace<std::decay_t<U>>(std::forward<U>(value));
            return *this;
        }

        /**
         * Retrieves a pointer to the stored value.
         *
         * This function accesses the stored value through the virtual table's
         * getValue method, returning a pointer of type T.
         *
         * @return A pointer to the stored value of type T.
         */
        constexpr T *get() {
            return vtable->getValue(storage);
        }

        /**
         * Retrieves a pointer to the stored value.
         *
         * This function accesses the stored value through the virtual table's
         * getValue method, returning a pointer of type T.
         *
         * @return A pointer to the stored value of type T.
         */
        constexpr const T *get() const {
            return vtable->getConstValue(storage);
        }

        /**
         * Provides access to the underlying pointer.
         *
         * Overloads the arrow operator to allow direct access to the pointer
         * managed by this object. It calls the `get()` method to retrieve the
         * underlying pointer.
         *
         * @return A pointer of type T* managed by this object.
         */
        constexpr T *operator->() {
            return get();
        }

        /**
         * Provides access to the underlying pointer.
         *
         * Overloads the arrow operator to allow direct access to the pointer
         * managed by this object. It calls the `get()` method to retrieve the
         * underlying pointer.
         *
         * @return A pointer of type T* managed by this object.
         */
        constexpr const T *operator->() const {
            return get();
        }

        /**
         * Dereference operator that returns a reference to the object managed by the current instance.
         *
         * This operator is used to access the value pointed to by the internal pointer or managed object.
         * It assumes that there is always a valid object to dereference, and it should be used where
         * the certainty of a managed object presence is guaranteed.
         *
         * @return A reference to the object of type T that is managed by the current instance.
         */
        constexpr T &operator*() {
            return *get();
        }

        /**
         * Dereference operator that returns a reference to the object managed by the current instance.
         *
         * This operator is used to access the value pointed to by the internal pointer or managed object.
         * It assumes that there is always a valid object to dereference, and it should be used where
         * the certainty of a managed object presence is guaranteed.
         *
         * @return A reference to the object of type T that is managed by the current instance.
         */
        constexpr const T &operator*() const {
            return *get();
        }

        /**
         * Replaces the current object stored in the polymorphic object storage
         * with a new instance of the specified derived type U, constructed
         * with the provided arguments.
         *
         * @tparam U The type of the new object to be stored. Must be a type derived from T.
         * @tparam A The types of the constructor arguments for the new object.
         * @param args The arguments to be forwarded to the constructor of the new object.
         *
         * @note This function first destroys the current object residing in the storage.
         *       It then checks if the new object fits in small storage. If it does,
         *       the object is constructed in-place within the small storage. Otherwise,
         *       it is allocated in the large storage.
         *
         * @requires U Must be a type derived from T.
         */
        template <typename U, typename... A>
            requires std::derived_from<U, T>
        constexpr void emplace(A &&...args) noexcept {
            vtable->destroy(storage);
            vtable = getVTable<U>();
            if constexpr (fitsSmallStorage<U>) {
                new (reinterpret_cast<U *>(storage.smallStorage.data())) U(std::forward<A>(args)...);
            } else {
                storage.largeStorage = new U(std::forward<A>(args)...);
            }
        }

        /**
         * Retrieves the size from the vtable.
         *
         * @return The size as a constant expression.
         */
        constexpr size_t getSize() const {
            return vtable->getSize();
        }

      private:
        union OpaqueStorage {
            std::array<std::byte, SmallStorageSize> smallStorage;
            void *largeStorage;

            template <typename U, typename... A>
                requires std::derived_from<U, T> && std::constructible_from<U, A...>
            constexpr void emplace(A &&...args) noexcept {
                if constexpr (fitsSmallStorage<U>) {
                    new (reinterpret_cast<U *>(smallStorage.data())) U(std::forward<A>(args)...);
                } else {
                    largeStorage = new U(std::forward<A>(args)...);
                }
            }
        };

        struct VTable {
            const std::type_info &(*getType)();
            size_t (*getSize)();
            T *(*getValue)(OpaqueStorage &storage);
            const T *(*getConstValue)(const OpaqueStorage &storage);
            void (*destroy)(OpaqueStorage &storage);
            void (*copy)(const OpaqueStorage &src, OpaqueStorage &dest);
            void (*copyAssign)(const OpaqueStorage &src, OpaqueStorage &dest);
            void (*move)(OpaqueStorage &src, OpaqueStorage &dest);
            void (*moveAssign)(OpaqueStorage &src, OpaqueStorage &dest);
        };

        template <typename U>
            requires std::derived_from<U, T>
        struct VTableImpl {
            static constexpr const std::type_info &getType() {
                return typeid(U);
            }

            static constexpr size_t getSize() {
                return sizeof(U);
            }

            static constexpr T *getValue(OpaqueStorage &data) {
                if constexpr (fitsSmallStorage<U>) {
                    return reinterpret_cast<U *>(data.smallStorage.data());
                } else {
                    return static_cast<U *>(data.largeStorage);
                }
            }

            static constexpr const T *getConstValue(const OpaqueStorage &data) {
                if constexpr (fitsSmallStorage<U>) {
                    return reinterpret_cast<const U *>(data.smallStorage.data());
                } else {
                    return static_cast<const U *>(data.largeStorage);
                }
            }

            static constexpr void destroy(OpaqueStorage &data) {
                if constexpr (fitsSmallStorage<U>) {
                    reinterpret_cast<const U *>(data.smallStorage.data())->~U();
                } else {
                    delete static_cast<const U *>(data.largeStorage);
                }
            }

            static constexpr void copy(const OpaqueStorage &src, OpaqueStorage &dest) {
                if constexpr (fitsSmallStorage<U>) {
                    dest.template emplace<U>(*reinterpret_cast<const U *>(src.smallStorage.data()));
                } else {
                    dest.template emplace<U>(*static_cast<const U *>(src.largeStorage));
                }
            }

            static constexpr void copyAssign(const OpaqueStorage &src, OpaqueStorage &dest) {
                if constexpr (fitsSmallStorage<U>) {
                    *reinterpret_cast<U *>(dest.smallStorage.data()) =
                        *reinterpret_cast<const U *>(src.smallStorage.data());
                } else {
                    *static_cast<U *>(dest.largeStorage) = *static_cast<const U *>(src.largeStorage);
                }
            }

            static constexpr void move(OpaqueStorage &src, OpaqueStorage &dest) {
                if constexpr (fitsSmallStorage<U>) {
                    dest.template emplace<U>(std::move(*reinterpret_cast<U *>(src.smallStorage.data())));
                } else {
                    dest.template emplace<U>(std::move(*static_cast<U *>(src.largeStorage)));
                }
            }

            static constexpr void moveAssign(OpaqueStorage &src, OpaqueStorage &dest) {
                if constexpr (fitsSmallStorage<U>) {
                    *reinterpret_cast<U *>(dest.smallStorage.data()) =
                        std::move(*reinterpret_cast<U *>(src.smallStorage.data()));
                } else {
                    *static_cast<U *>(dest.largeStorage) = std::move(*static_cast<U *>(src.largeStorage));
                }
            }
        };

        template <typename U>
            requires std::derived_from<U, T>
        static const VTable *getVTable() {
            using ImplType = VTableImpl<U>;
            static constexpr VTable vtable = {.getType = &ImplType::getType,
                                              .getSize = &ImplType::getSize,
                                              .getValue = &ImplType::getValue,
                                              .getConstValue = &ImplType::getConstValue,
                                              .destroy = &ImplType::destroy,
                                              .copy = &ImplType::copy,
                                              .copyAssign = &ImplType::copyAssign,
                                              .move = &ImplType::move,
                                              .moveAssign = &ImplType::moveAssign};
            return &vtable;
        }

        OpaqueStorage storage;
        const VTable *vtable = getVTable<T>();
    };

} // namespace Retro
