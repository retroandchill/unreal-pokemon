// "Unreal Pokémon" created by Retro & Chill.


#include "Utilities/PokemonUIUtils.h"

FString UPokemonUIUtils::ZeroPad(int32 Value, int32 DesiredLength) {
	return PadInt(Value, DesiredLength);
}

FString UPokemonUIUtils::SpacePad(int32 Value, int32 DesiredLength) {
	return PadInt(Value, DesiredLength, TEXT(' '));
}

FString UPokemonUIUtils::PadInt(int32 Value, int32 DesiredLength, TCHAR PaddingCharacter) {
	auto AsString = FString::FromInt(Value);
	int32 Diff = DesiredLength - AsString.Len();
	if (Diff <= 0)
		return AsString;

	FString Output(TEXT(""), FMath::Max(DesiredLength, AsString.Len()));
	for (int i = 0; i < Diff; i++) {
		Output.AppendChar(PaddingCharacter);
	}

	Output.Append(AsString);
	return Output;
}
