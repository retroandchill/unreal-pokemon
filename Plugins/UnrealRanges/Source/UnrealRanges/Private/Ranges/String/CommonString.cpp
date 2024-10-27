// "Unreal Pokémon" created by Retro & Chill.

#include "Ranges/String/CommonString.h"

namespace UE::Ranges {
    FCommonString ExtractCommonStringFromProperty(const FProperty *Property, const uint8 *Data) {
        FStringPropertyVisitor Visitor(Property);
        return Visitor.Visit(Data, []<typename T>(T &&Str) { return GetString(std::forward<T>(Str)); });
    }

    FStringView GetStringView(const FCommonString &String) {
        return std::visit([]<typename T>(T &&Str) { return GetStringView(std::forward<T>(Str)); }, String);
    }
} // namespace UE::Ranges