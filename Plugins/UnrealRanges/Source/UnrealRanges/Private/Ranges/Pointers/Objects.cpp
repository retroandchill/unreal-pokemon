// "Unreal Pokémon" created by Retro & Chill.

#include "Ranges/Pointers/Objects.h"
#include "Ranges/Optional/GetPtrOrNull.h"
#include "Ranges/Optional/Map.h"
#include "Ranges/Optional/OptionalClosure.h"
#include "Ranges/Optional/OptionalRef.h"

UObject *UE::Ranges::GetObjectFromProperty(const FProperty *Property, const uint8 *Data) {
    FObjectPropertyVisitor Visitor(Property);
    return Visitor.Visit(Data, []<typename T>(T &&Obj) { return GetObject(std::forward<T>(Obj)); });
}

void UE::Ranges::AssignObjectToProperty(const FProperty *Property, uint8 *Data, UObject *Object) {
    FObjectPropertyVisitor::Visit(Property, [Data, Object](const auto *Prop) { SetObject(Prop, Data, Object); });
}

void UE::Ranges::SetObject(const FObjectProperty *ObjectProperty, uint8 *const Storage, UObject *Object) {
    ObjectProperty->SetObjectPropertyValue(Storage, Object);
}

void UE::Ranges::SetObject(const FInterfaceProperty *ObjectProperty, uint8 *const Storage, UObject *Object) {
    auto Interface = FInterfaceProperty::GetPropertyValuePtr(Storage);
    Interface->SetObject(Object);
    // clang-format off
    auto Value = Optionals::OfNullable(Object) |
                 Optionals::Map([ObjectProperty](UObject& Obj) { return Obj.GetNativeInterfaceAddress(ObjectProperty->InterfaceClass); }) |
                 Optionals::GetPtrOrNull;
    // clang-format on
    Interface->SetInterface(Value);
}