// Fill out your copyright notice in the Description page of Project Settings.

#ifdef __UNREAL__
#include "RetroLib/Blueprints/BlueprintRuntimeUtils.h"

#include "RetroLib/Exceptions/TypeException.h"
#include "RetroLib/Exceptions/VariantException.h"
#include "RetroLib/Variants/VariantObjectStruct.h"

void Retro::ValidateStructsMatch(const FStructProperty &Property, const UScriptStruct *Struct) {
	if (Property.Struct != Struct) {
		throw FTypeException("Incompatible output parameter; the supplied struct does not have the "
									   "same layout as what is expected for a variant object struct.");
	}
}

void Retro::ValidateEnumsMatch(const FEnumProperty &Property, const UEnum *Enum) {
	if (Property.GetEnum() != Enum) {
		throw FTypeException("Incompatible output parameter; the supplied enum is not the same type as the"
									   "expected type.");
	}
}

Retro::IVariantRegistration &Retro::GetVariantRegistration(const FStructProperty &Property) {
	static auto &Registry = FVariantObjectStructRegistry::Get();
	auto StructInfo = Registry.GetVariantStructData(*Property.Struct);
	if (!StructInfo.IsSet()) {
		throw FVariantException("Incompatible struct type; the supplied struct is not a registered variant struct.");
	}

	return *StructInfo;
}
#endif