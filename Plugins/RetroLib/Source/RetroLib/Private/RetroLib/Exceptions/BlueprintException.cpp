// Fill out your copyright notice in the Description page of Project Settings.

#ifdef __UNREAL__
#include "RetroLib/Exceptions/BlueprintException.h"


Retro::FBlueprintException::FBlueprintException(const char* Message, EBlueprintExceptionType::Type Type) : Message(Message), Type(Type)  {
}

Retro::FBlueprintException::FBlueprintException(std::string_view Message, EBlueprintExceptionType::Type Type) : Message(Message), Type(Type) {}

Retro::FBlueprintException::FBlueprintException(std::string &&Message, EBlueprintExceptionType::Type Type) : Message(std::move(Message)), Type(Type) {}

const char* Retro::FBlueprintException::what() const {
	return Message.data();
}
#endif