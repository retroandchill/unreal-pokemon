// "Unreal Pokémon" created by Retro & Chill.
#include "Data/SelectionInputs.h"

TOptional<ECursorDirection> USelectionInputs::ParseDirectionalInputs(const FKey& Key) const {
	using enum ECursorDirection;
	if (UpInputs.Contains(Key)) {
			return Up;
	}

	if (DownInputs.Contains(Key)) {
		return Down;
	}

	if (LeftInputs.Contains(Key)) {
		return Left;
	}

	if (RightInputs.Contains(Key)) {
		return Right;
	}
	
	return TOptional<ECursorDirection>();
}

bool USelectionInputs::IsConfirmInput(const FKey& Key) const {
	return ConfirmInputs.Contains(Key);
}

bool USelectionInputs::IsCancelInput(const FKey& Key) const {
	return CancelInputs.Contains(Key);
}
