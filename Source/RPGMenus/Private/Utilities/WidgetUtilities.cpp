// "Unreal Pokémon" created by Retro & Chill.
#include "Utilities/WidgetUtilities.h"

#include "Engine/UserInterfaceSettings.h"

float UWidgetUtilities::GetWidgetDPIScale() {
	static constexpr float SlateDPI = 96.f;
	auto FontDPI = static_cast<float>(GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetFontDisplayDPI());
	return SlateDPI / FontDPI;
}
