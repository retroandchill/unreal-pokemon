// "Unreal Pokémon" created by Retro & Chill.


#include "Settings/GameSettings.h"
#include "Settings/BaseSettings.h"

TScriptInterface<IGameSettings> CreateGameSettingsWrapper() {
    TScriptInterface<IGameSettings> Interface = UGameSettings::StaticClass();
    Interface.SetInterface(&Pokemon::FBaseSettings::Get());
    return Interface;
}

const TScriptInterface<IGameSettings> &UGameSettingsFunctionLibrary::GetGameSettings() {
    static auto Interface = CreateGameSettingsWrapper();
    return Interface;
}