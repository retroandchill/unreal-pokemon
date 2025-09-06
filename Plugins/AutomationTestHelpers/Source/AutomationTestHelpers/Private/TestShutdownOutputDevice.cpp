// "Unreal Pokémon" created by Retro & Chill.

#include "TestShutdownOutputDevice.h"

void FTestShutdownOutputDevice::Serialize(const TCHAR *V, ELogVerbosity::Type Verbosity, const FName &Category)
{
    if (FCString::Stristr(V, TEXT("Automation Test Queue Empty")))
    {
        bShutdownMessageReceived = true;
    }
}

bool FTestShutdownOutputDevice::ShutdownMessageReceived() const
{
    return bShutdownMessageReceived;
}