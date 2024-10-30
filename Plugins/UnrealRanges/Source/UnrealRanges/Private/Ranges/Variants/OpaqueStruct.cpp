// "Unreal Pokémon" created by Retro & Chill.


#include "Ranges/Variants/OpaqueStruct.h"

using namespace UE::Ranges;

FOpaqueStruct::FOpaqueStruct(const UScriptStruct *Struct) noexcept : Storage(MakeStorage(Struct)) {
}

FOpaqueStruct::FOpaqueStruct(const FOpaqueStruct &Other) : Storage(Other.PerformCopy()) {
}

FOpaqueStruct &FOpaqueStruct::operator=(const FOpaqueStruct &Other) {
    Storage = Other.PerformCopy();
    return *this;
}

void * FOpaqueStruct::GetValue() const {
    auto Struct = GetStruct();
    check(IsValid(Struct))
    return Storage.get();
}

TOptional<void> FOpaqueStruct::TryGet() const {
    if (!IsValid(GetStruct())) {
        return nullptr;
    }

    return GetValue();
}

void FOpaqueStruct::Swap(FOpaqueStruct &Other) {
    std::swap(Storage, Other.Storage);
}

FOpaqueStruct::FStorage FOpaqueStruct::MakeStorage(const UScriptStruct *Struct) noexcept {
    if (!IsValid(Struct)) {
        return FStorage();
    }

    auto AllocateStruct = [](const UScriptStruct *S) {
        auto Memory = FMemory::Malloc(static_cast<size_t>(S->GetStructureSize()));
        S->InitializeStruct(Memory);
        return Memory;
    };
    
    return FStorage(AllocateStruct(Struct), FStructDeleter(Struct));
}

FOpaqueStruct::FStorage FOpaqueStruct::PerformCopy() const {
    auto Struct = GetStruct();
    if (!IsValid(Struct)) {
        return FStorage();
    }

    auto CopyStruct = [](const FStorage& Other) {
        auto &OtherStruct = Other.get_deleter().Struct;
        auto Memory = FMemory::Malloc(static_cast<size_t>(OtherStruct->GetStructureSize()));
        OtherStruct->CopyScriptStruct(Memory, OtherStruct.Get());
        return Memory;
    };
    
    return FStorage(CopyStruct(Storage), FStructDeleter(Struct));
}