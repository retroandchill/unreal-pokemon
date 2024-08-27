#pragma once

#include "CoreMinimal.h"

enum class EGridMapEditingState : uint8
{
	Unknown,
	Enabled,
	PIEWorld,
	SIEWorld,
};

enum class EGridMapPaintMode : uint8
{
	Paint,
	Erase,
};
