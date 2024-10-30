// "Unreal Pokémon" created by Retro & Chill.


#include "Ranges/Variants/OpaqueStruct.h"

using namespace UE::Ranges;

FOpaqueStruct::FStorage::FStorage(const UScriptStruct *Struct) {
    if (auto StructureSize = static_cast<size_t>(Struct->GetStructureSize()); StructureSize <= SmallBufferSize) {
        Struct->InitializeStruct(SmallStorage.data());
    } else {
        LargeStorage = FMemory::Malloc(StructureSize);
        Struct->InitializeStruct(LargeStorage);
    }
}

FOpaqueStruct::FOpaqueStruct(const UScriptStruct *Struct) noexcept : Storage(Struct), Struct(Struct) {
}

FOpaqueStruct::~FOpaqueStruct() {
    DeleteExisting();
}

FOpaqueStruct::FOpaqueStruct(const FOpaqueStruct &Other) : Storage(Other.PerformCopy()), Struct(Other.Struct) {
}

FOpaqueStruct::FOpaqueStruct(FOpaqueStruct &&Other) noexcept : Storage(std::move(Other.Storage)),
                                                      Struct(std::move(Other.Struct)) {
    Other.Struct = nullptr;
}

FOpaqueStruct &FOpaqueStruct::operator=(const FOpaqueStruct &Other) {
    DeleteExisting();
    Storage = Other.PerformCopy();
    Struct = Other.Struct;
    return *this;
}

FOpaqueStruct &FOpaqueStruct::operator=(FOpaqueStruct &&Other) noexcept {
    DeleteExisting();
    Storage = std::move(Other.Storage);
    Struct = std::move(Other.Struct);
    Other.Struct = nullptr;
    return *this;
}

void * FOpaqueStruct::GetValue() const {
    check(Struct != nullptr)
    if (Struct->GetStructureSize() >= SmallBufferSize) {
        return const_cast<std::byte*>(Storage.SmallStorage.data());
    }
    
    return Storage.LargeStorage;
}

TOptional<void> FOpaqueStruct::TryGet() const {
    if (Struct == nullptr) {
        return nullptr;
    }

    return GetValue();
}

void FOpaqueStruct::Swap(FOpaqueStruct &Other) {
    std::swap(Struct, Other.Struct);
}

void FOpaqueStruct::DeleteExisting() {
    if (Struct == nullptr) {
        return;
    }
    
    if (Struct->GetStructureSize() <= SmallBufferSize) {
        Struct->DestroyStruct(Storage.SmallStorage.data());
    } else {
        Struct->DestroyStruct(Storage.LargeStorage);
        FMemory::Free(Storage.LargeStorage);
    }
}

FOpaqueStruct::FStorage FOpaqueStruct::PerformCopy() const {
    FStorage NewStorage;
    if (auto StructureSize = static_cast<size_t>(Struct->GetStructureSize()); StructureSize <= SmallBufferSize) {
        Struct->CopyScriptStruct(NewStorage.SmallStorage.data(), Storage.SmallStorage.data());
    } else {
        NewStorage.LargeStorage = FMemory::Malloc(StructureSize);
        Struct->CopyScriptStruct(NewStorage.LargeStorage, Storage.LargeStorage);
    }
    return NewStorage;
}