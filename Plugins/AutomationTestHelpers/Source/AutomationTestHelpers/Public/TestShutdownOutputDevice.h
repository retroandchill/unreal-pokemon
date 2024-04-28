// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 * This class is a custom output device for capturing log messages.
 */
class AUTOMATIONTESTHELPERS_API FTestShutdownOutputDevice : public FOutputDevice {
  public:
    void Serialize(const TCHAR *V, ELogVerbosity::Type Verbosity, const FName &Category) override;

    /**
     * @brief Check if the shutdown message has been received.
     *
     * This method returns a boolean value indicating whether the shutdown message
     * has been received or not.
     *
     * @return True if the shutdown message has been received, false otherwise.
     */
    bool ShutdownMessageReceived() const;

  private:
    /**
     * @brief Flag indicating whether the shutdown message has been received.
     *
     * This variable is a boolean flag that indicates whether the shutdown message
     * has been received or not. It is initially set to false and gets set to true
     * when the shutdown message is detected.
     */
    bool bShutdownMessageReceived = false;
};
