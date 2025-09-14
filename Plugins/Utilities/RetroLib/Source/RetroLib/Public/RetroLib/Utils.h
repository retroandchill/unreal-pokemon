// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RetroLib/Utils/Construct.h"
#include "RetroLib/Utils/ForwardLike.h"
#include "RetroLib/Utils/Math.h"
#include "RetroLib/Utils/NonPropagatingCache.h"
#include "RetroLib/Utils/Operators.h"
#include "RetroLib/Utils/Polymorphic.h"
#include "RetroLib/Utils/Tuple.h"
#include "RetroLib/Utils/UniqueAny.h"
#include "RetroLib/Utils/Unreachable.h"
#include "RetroLib/Utils/ValidPtr.h"
#include "RetroLib/Utils/Variant.h"

#ifdef __UNREAL__
#include "RetroLib/Utils/BlueprintMathUtils.h"
#include "RetroLib/Utils/CommonString.h"
#include "RetroLib/Utils/DateUtils.h"
#include "RetroLib/Utils/GameplayTagUtilities.h"
#include "RetroLib/Utils/OpaqueStruct.h"
#include "RetroLib/Utils/ScopedTimeDilationFactor.h"
#include "RetroLib/Utils/SoftObjectRef.h"
#include "RetroLib/Utils/StringUtilities.h"
#include "RetroLib/Utils/WorldUtils.h"
#endif