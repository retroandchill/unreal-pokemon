// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Serialization.h"
#include <array>
#include <variant>

namespace UE::Ranges {

    /**
     * Serializes the given data to or from the provided archive.
     *
     * This function supports both loading from and saving to an archive. It takes
     * advantage of the serialization operator `<<` to process the data.
     * The function is constrained to types that satisfy the `CanSerialize` concept.
     *
     * @tparam T The type of the data to be serialized.
     * @param Archive The archive object used for serialization.
     * @param Data The data to be serialized or deserialized.
     */
    template <typename T>
        requires CanSerialize<T>
    void SerializeData(FArchive &Archive, T &Data) {
        Archive << Data;
    }

    /**
     * Deserializes a specific type `T` into a given `std::variant<A...>`.
     *
     * This function emplaces the variant with the type `T` and then deserializes
     * the corresponding data from the provided archive into the variant.
     *
     * @tparam T The type to be deserialized into the variant.
     * @tparam A The types that the variant can hold.
     * @param Archive The archive object used for deserialization.
     * @param Variant The variant that will hold the deserialized data of type `T`.
     */
    template <typename T, typename... A>
        requires CanSerialize<T> && (std::same_as<T, A> || ...)
    void LoadVariantData(FArchive &Archive, std::variant<A...> &Variant) {
        Variant.template emplace<T>();
        SerializeData(Archive, std::get<T>(Variant));
    }

} // namespace UE::Ranges

/**
 * Stream operator overload for saving and loading data from std::variant.
 *
 * @tparam T the type of data contained within the variant
 * @param Archive The archive that we are saving to/loading from
 * @param Variant The variant data in question
 * @return The archive that we are saving to/loading from
 */
template <typename... T>
    requires(UE::Ranges::CanSerialize<T> && ...)
FArchive &operator<<(FArchive &Archive, std::variant<T...> &Variant) {
    auto TypeIndex = static_cast<size_t>(INDEX_NONE);
    if (Archive.IsLoading()) {
        static constexpr std::array LoadFunctions = {&UE::Ranges::LoadVariantData<T, T...>...};
        Archive << TypeIndex;
        check(TypeIndex < LoadFunctions.size())
        LoadFunctions[TypeIndex](Archive, Variant);
    } else {
        TypeIndex = Variant.index();
        Archive << TypeIndex;
        std::visit([&Archive]<typename U>(U &Data) { UE::Ranges::SerializeData<U>(Archive, Data); }, Variant);
    }

    return Archive;
}