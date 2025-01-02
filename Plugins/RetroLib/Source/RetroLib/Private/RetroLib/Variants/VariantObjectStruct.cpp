// Fill out your copyright notice in the Description page of Project Settings.


#ifdef __UNREAL__
#include "RetroLib/Variants/VariantObjectStruct.h"

#include "RetroLib/Optionals/Transform.h"


namespace Retro {
	FVariantObjectStructRegistry &FVariantObjectStructRegistry::Get() {
		static FVariantObjectStructRegistry Instance;
		return Instance;
	}

	TOptional<IVariantRegistration &> FVariantObjectStructRegistry::GetVariantStructData(const UScriptStruct &Struct) {
		static_assert(IsValidFunctorObject(&TSharedRef<IVariantRegistration>::Get));
		return Optionals::OfNullable<TOptional>(RegisteredStructs.Find(Struct.GetFName())) |
			   Optionals::Transform<&TSharedRef<IVariantRegistration>::Get>();
	}
} // namespace Retro
#endif