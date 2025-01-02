// Fill out your copyright notice in the Description page of Project Settings.

#ifdef __UNREAL__
#pragma once

#include "RetroLib/Exceptions/ConvertException.h"

#include "RetroLib/Exceptions/BlueprintException.h"


FBlueprintExceptionInfo Retro::ConvertException(const std::exception& Exception, EBlueprintExceptionType::Type Type) {
	return FBlueprintExceptionInfo(Type, FText::FromString(Exception.what()));
}

FBlueprintExceptionInfo Retro::ConvertException(const FBlueprintException& Exception) {
	return ConvertException(Exception, Exception.GetType());
}
#endif
