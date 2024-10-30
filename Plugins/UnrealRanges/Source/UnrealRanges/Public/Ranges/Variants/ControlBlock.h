// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {
    
    /**
     * @struct TTypedInitializer
     *
     * @brief The TTypedInitializer struct serves as a tag type used to specify a type for which various lifecycle management functionality is being initialized.
     *
     * This tag type is used in conjunction with other classes, such as FControlBlock, to indicate the specific type T that is under consideration for initialization, construction, destruction, copying, and moving operations.
     */
    template <typename T>
        requires std::default_initializable<T> && std::copyable<T> && std::movable<T>
    struct TTypedInitializer {
    };

    /**
     * @class FControlBlock
     *
     * @brief The FControlBlock class is responsible for managing the construction and deletion of an opaque data type.
     */
    struct FControlBlock {

        /**
         * The signature of the functions used for calls to the constructor/destructor.
         */
        using SingleOperator = void(*)(void *);

        /**
         * The signature of the functions used for copy construction/assignment.
         */
        using CopyOperator = void(*)(void *, const void *);

        /**
         * The signature of the functions used for move construction/assignment.
         */
        using MoveOperator = void(*)(void *, void *);

        /**
         * The size of the type.
         */
        size_t Size;

        /**
         * The alignment of the type.
         */
        size_t Alignment;

        /**
         * The default constructor of the type.
         */
        SingleOperator DefaultConstructor;

        /**
         * The destructor of the type.
         */
        SingleOperator Destructor;

        /**
         * The copy constructor of the type.
         */
        CopyOperator CopyConstructor;

        /**
         * The move constructor of the type.
         */
        MoveOperator MoveConstructor;

        /**
         * The copy assignment operator of the type.
         */
        CopyOperator CopyAssignment;

        /**
         * The move assignment operator of the type.
         */
        MoveOperator MoveAssignment;
        
        /**
         * @brief Constructs an FControlBlock for a given type T.
         *
         * This constructor initializes the size, alignment, and various function pointers
         * for managing the lifecycle (construction, destruction, copying, moving) of the type T.
         *
         * @tparam T The type that is being initialized. T must satisfy the requirements of being default initializable, copyable, and movable.
         * @param Initializer A tag type used to specify the type T for which the control block is being initialized.
         *                             
         */
        template <typename T>
            requires std::default_initializable<T> && std::copyable<T> && std::movable<T>
        explicit FControlBlock([[maybe_unused]] TTypedInitializer<T> Initializer) : Size(sizeof(T)),
            Alignment(alignof(T)), DefaultConstructor(&FControlBlock::Construct<T>),
            Destructor(&FControlBlock::Destruct<T>), CopyConstructor(&FControlBlock::CopyToNew<T>),
            MoveConstructor(&FControlBlock::MoveToNew<T>), CopyAssignment(&FControlBlock::CopyToExisting<T>),
            MoveAssignment(&FControlBlock::MoveToExisting<T>) {
        }

    private:
        /**
         * @brief Constructs an instance of type T at the specified memory location.
         *
         * This method uses placement new to construct an object of type T
         * at the location specified by the Destination pointer.
         *
         * @param Destination A pointer to the memory location where the object of type T should be constructed.
         */
        template <typename T>
        static void Construct(void *Destination) {
            new(Destination) T();
        }

        /**
         * @brief Destructs an instance of type T located at the specified memory location.
         *
         * This method explicitly calls the destructor of the object of type T at the memory
         * location pointed to by Data.
         *
         * @param Data A pointer to the memory location where the object of type T should be destructed.
         *             It is assumed that Data points to a valid instance of T.
         */
        template <typename T>
        static void Destruct(void *Data) {
            static_cast<T *>(Data)->~T();
        }

        /**
         * @brief Constructs a copy of an object of type T at the specified memory location.
         *
         * This method uses placement new to construct an object of type T at the memory location
         * specified by Destination, using the object pointed to by Source.
         *
         * @param Destination A pointer to the memory location where the object of type T should be constructed.
         * @param Source A pointer to the object of type T from which a copy will be made.
         */
        template <typename T>
        static void CopyToNew(void *Destination, const void *Source) {
            new(Destination) T(*static_cast<const T *>(Source));
        }

        /**
         * @brief Copies a value of type T from the source memory location to an existing destination memory location.
         *
         * This function performs a copy assignment from a source object to an existing destination object of type T.
         *
         * @tparam T The type of the object being copied.
         * @param Destination A pointer to the memory location where the value should be copied to. It must already contain a valid instance of T.
         * @param Source A pointer to the memory location from which the value should be copied. It must point to a valid instance of T.
         */
        template <typename T>
        static void CopyToExisting(void *Destination, const void *Source) {
            *static_cast<T *>(Destination) = *static_cast<const T *>(Source);
        }

        /**
         * @brief Constructs a new object of type T at the specified destination by moving the object from the source.
         *
         * @param Destination Pointer to the location where the new object will be constructed.
         * @param Source Pointer to the object that will be moved.
         */
        template <typename T>
        static void MoveToNew(void *Destination, void *Source) {
            new(Destination) T(std::move(*static_cast<T *>(Source)));
        }

        /**
         * @brief Replaces the object at the destination with the object at the source.
         *
         * This function moves the object from the source pointer to the destination
         * pointer using the move assignment operator.
         *
         * @param Destination Pointer to the destination object.
         * @param Source Pointer to the source object.
         */
        template <typename T>
        static void MoveToExisting(void *Destination, void *Source) {
            *static_cast<T *>(Destination) = std::move(*static_cast<T *>(Source));
        }

    };
}