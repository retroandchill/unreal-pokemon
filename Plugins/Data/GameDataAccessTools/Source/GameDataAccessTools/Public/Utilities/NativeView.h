// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
template <typename T>
class TNativeView
{
  public:
    TNativeView() = default;

    explicit TNativeView(const T &InData) : Data(&InData)
    {
    }

    const T &Get() const
    {
        return *Data;
    }

  private:
    const T *Data = nullptr;
};
