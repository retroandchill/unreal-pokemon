// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifdef __UNREAL__
#if !RETROLIB_WITH_MODULES
#include "UObject/Class.h"
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {
	RETROLIB_EXPORT template <typename T>
	concept CoreStructType = requires {
		{ TBaseStructure<std::remove_cvref_t<T>>::Get() } -> std::same_as<UScriptStruct *>;
	};

	/**
	 * Concept for any USTRUCT in the editor.
	 */
	RETROLIB_EXPORT template <typename T>
	concept DeclaredStruct = requires {
		{ std::remove_cvref_t<T>::StaticStruct() } -> std::same_as<UScriptStruct *>;
	};

	RETROLIB_EXPORT template <typename T>
	concept UEStruct = CoreStructType<T> || DeclaredStruct<T>;

	RETROLIB_EXPORT template <typename T>
		requires UEStruct<T>
	constexpr UScriptStruct *GetScriptStruct() {
		if constexpr (Retro::DeclaredStruct<T>) {
			return T::StaticStruct();
		} else {
			return TBaseStructure<T>::Get();
		}
	}
}
#endif