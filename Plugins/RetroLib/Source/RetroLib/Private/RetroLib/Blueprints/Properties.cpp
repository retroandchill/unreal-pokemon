// Fill out your copyright notice in the Description page of Project Settings.

#ifdef __UNREAL__
#include "RetroLib/Blueprints/Properties.h"

#include <RetroLib/Optionals/OrElseValue.h>
#include <RetroLib/Optionals/Transform.h>
#include "RetroLib/Optionals/OptionalOperations.h"


UObject *Retro::GetObjectFromProperty(const FProperty *Property, const uint8 *Data) {
	FObjectPropertyVisitor Visitor(Property);
	return Visitor.Visit(Data, []<typename T>(T &&Obj) { return GetObject(std::forward<T>(Obj)); });
}

void Retro::AssignObjectToProperty(const FProperty *Property, uint8 *Data, UObject *Object) {
	FObjectPropertyVisitor::Visit(Property, [Data, Object](const auto *Prop) { SetObject(Prop, Data, Object); });
}

void Retro::SetObject(const FObjectProperty *ObjectProperty, uint8 *const Storage, UObject *Object) {
	ObjectProperty->SetObjectPropertyValue(Storage, Object);
}

void Retro::SetObject(const FInterfaceProperty *ObjectProperty, uint8 *const Storage, UObject *Object) {
	auto Interface = FInterfaceProperty::GetPropertyValuePtr(Storage);
	Interface->SetObject(Object);
	// clang-format off
	auto Value = Optionals::OfNullable(Object) |
				 Optionals::Transform([ObjectProperty](UObject& Obj) { return Obj.GetNativeInterfaceAddress(ObjectProperty->InterfaceClass); }) |
				 Optionals::OrElseValue(nullptr);
	// clang-format on
	Interface->SetInterface(Value);
}
#endif