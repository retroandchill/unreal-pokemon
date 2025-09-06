// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedSaveGameModule.h"

#if WITH_EDITOR
namespace EnhancedSaveGame
{
    /**
     * @class FScopedInMemorySaveHandle
     *
     * @brief Manages a scoped save handle for in-memory data operations.
     *
     * The FScopedInMemorySaveHandle class is designed to temporarily enable in-memory saving, returning to regular,
     * file-based saving when this object is destructed.
     */
    class FScopedInMemorySaveHandle
    {
      public:
        /**
         * @brief Constructor that enables in-memory saving.
         *
         * This constructor calls EnableInMemorySaving on FEnhancedSaveGameModule to
         * switch the save system to use in-memory saving for the duration of this object's lifecycle.
         */
        FScopedInMemorySaveHandle()
        {
            FEnhancedSaveGameModule::Get().EnableInMemorySaving();
        }

        FScopedInMemorySaveHandle(const FScopedInMemorySaveHandle &) = delete;

        /**
         * @brief Move constructor for FScopedInMemorySaveHandle.
         *
         * Transfers the state of in-memory saving from one instance to another.
         *
         * @param Other The instance from which to move the state.
         *
         * @return A new instance of FScopedInMemorySaveHandle with moved state.
         */
        FScopedInMemorySaveHandle(FScopedInMemorySaveHandle &&Other) noexcept
        {
            Other.bIsActive = false;
        }

        /**
         * @brief Destructor that disables in-memory saving.
         *
         * This destructor calls DisableInMemorySaving on FEnhancedSaveGameModule to
         * revert the save system back to regular, file-based saving after in-memory saving
         * was enabled during the lifecycle of this object.
         */
        ~FScopedInMemorySaveHandle()
        {
            if (bIsActive)
            {
                FEnhancedSaveGameModule::Get().DisableInMemorySaving();
            }
        }

        FScopedInMemorySaveHandle &operator=(const FScopedInMemorySaveHandle &) = delete;

        /**
         * @brief Move assignment operator for FScopedInMemorySaveHandle.
         *
         * Transfers the state of in-memory saving from one instance to another.
         *
         * @param Other The instance from which to move the state.
         *
         * @return A new instance of FScopedInMemorySaveHandle with moved state.
         */
        FScopedInMemorySaveHandle &operator=(FScopedInMemorySaveHandle &&Other) noexcept
        {
            bIsActive = true;
            Other.bIsActive = false;
            return *this;
        }

      private:
        bool bIsActive = true;
    };
} // namespace EnhancedSaveGame
#endif