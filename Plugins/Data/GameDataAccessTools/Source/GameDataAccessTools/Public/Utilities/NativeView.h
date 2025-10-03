// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 * Thin wrapper around a native pointer to an object. It is meant to serve as a base class for some USTRUCT proxies to
 * C# code.
 *
 * @tparam T The type of data we are observing.
 */
template <typename T>
class TNativeView
{
  public:
    /**
     * Default constructor for the TNativeView class.
     *
     * Initializes an empty instance of TNativeView that does not point to an object.
     */
    TNativeView() = default;

    /**
     * Constructs an instance of TNativeView that points to the provided data.
     *
     * @param InData A constant reference to the object that this TNativeView should point to.
     */
    explicit TNativeView(const T &InData) : Data(&InData)
    {
    }

    /**
     * Retrieves a constant reference to the object pointed to by this instance.
     *
     * @return A constant reference to the object managed by this TNativeView instance.
     *         If the instance does not point to an object, the behavior is undefined.
     */
    const T &Get() const
    {
        return *Data;
    }

  private:
    const T *Data = nullptr;
};
