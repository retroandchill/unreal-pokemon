// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "BlueprintException.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

#ifdef __UNREAL__
namespace Retro {
	/**
	 * 
	 */
	RETROLIB_EXPORT class RETROLIB_API FTypeException : public FBlueprintException {
	public:
		using FBlueprintException::FBlueprintException;
	};
}
#endif