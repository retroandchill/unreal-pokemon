// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#ifdef PI
#define PI_WAS_DEFINED (PI)
#undef PI
#endif

THIRD_PARTY_INCLUDES_START
#include "range/v3/all.hpp"
THIRD_PARTY_INCLUDES_END

#include "UnrealContainers.h"

#ifdef PI_WAS_DEFINED
#define PI (PI_WAS_DEFINED)
#endif