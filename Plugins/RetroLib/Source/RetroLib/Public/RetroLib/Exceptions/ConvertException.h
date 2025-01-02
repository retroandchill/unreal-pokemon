// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifdef __UNREAL__
#if !RETROLIB_WITH_MODULES
#include <Blueprint/BlueprintExceptionInfo.h>
#include <exception>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {
	RETROLIB_API RETROLIB_EXPORT FBlueprintExceptionInfo ConvertException(const std::exception& Exception, EBlueprintExceptionType::Type Type = EBlueprintExceptionType::AccessViolation);

	RETROLIB_API RETROLIB_EXPORT FBlueprintExceptionInfo ConvertException(const class FBlueprintException& Exception);
}
#endif