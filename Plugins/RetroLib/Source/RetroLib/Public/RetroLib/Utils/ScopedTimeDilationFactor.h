// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#ifdef __UNREAL__

#if !RETROLIB_WITH_MODULES
#include "UObject/Object.h"
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {
    /**
     * @class FScopedTimeDilationFactor
     *
     * @brief A utility class that manages the time dilation factor within its scope.
     *
     * This class is designed to temporarily set a time dilation factor for a particular
     * context, reverting to the previous value when it goes out of scope. This is useful
     * in scenarios where a temporary change in time perception is required, such as slow-motion effects.
     *
     * @details The time dilation factor affects the rate at which time passes in the
     * simulation or game. By setting a new factor upon creation, the class ensures
     * that the old factor is restored once the instance is destroyed, maintaining the
     * consistency of the time dilation state.
     */
    class RETROLIB_API FScopedTimeDilationFactor : public FGCObject {

      public:
        /**
         * @brief Constructs an FScopedTimeDilationFactor instance and sets the time dilation factor.
         *
         * @param WorldContextObject The context object which provides the world information.
         * @param Factor The time dilation factor to be applied, affecting the simulation speed.
         *
         * @return An instance of FScopedTimeDilationFactor that manages the time dilation within its scope.
         */
        FScopedTimeDilationFactor(const UObject *WorldContextObject, float Factor) noexcept;

        FScopedTimeDilationFactor(const FScopedTimeDilationFactor &) = delete;
        FScopedTimeDilationFactor(FScopedTimeDilationFactor &&) noexcept = delete;

        /**
         * @brief Destructor for FScopedTimeDilationFactor.
         *
         * Restores the original time dilation factor when the destructor is called. This ensures
         * that any temporary changes to the simulation speed are reverted to their previous state,
         * maintaining the consistency of the time dilation state.
         *
         * The destructor checks if the WorldContextObject is valid before attempting to reset the global
         * time dilation.
         */
        ~FScopedTimeDilationFactor() override;

        FScopedTimeDilationFactor &operator=(const FScopedTimeDilationFactor &) = delete;
        FScopedTimeDilationFactor &operator=(FScopedTimeDilationFactor &&) noexcept = delete;

        void AddReferencedObjects(FReferenceCollector &Collector) override;
        FString GetReferencerName() const override;

      private:
        TObjectPtr<const UObject> WorldContextObject;
        float Factor;
    };
} // namespace Retro
#endif