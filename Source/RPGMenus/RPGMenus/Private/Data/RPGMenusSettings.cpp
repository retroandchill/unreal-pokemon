// "Unreal Pokémon" created by Retro & Chill.


#include "Data/RPGMenusSettings.h"

USoundBase* URPGMenusSettings::GetCursorSound() const {
	return Cast<USoundBase>(CursorSound.TryLoad());
}

USoundBase* URPGMenusSettings::GetConfirmSound() const {
	return Cast<USoundBase>(ConfirmSound.TryLoad());
}

USoundBase* URPGMenusSettings::GetCancelSound() const {
	return Cast<USoundBase>(CancelSound.TryLoad());
}
