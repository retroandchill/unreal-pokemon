// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Utilities/Casts.h"

namespace UE::Ranges {
    template <typename T>
        requires std::derived_from<T, UObject>
    class TClassViewIterator {
    public:
        using SizeType = typename TArray<TSubclassOf<T>>::SizeType;
        using difference_type = std::ptrdiff_t;
        using value_type = TSubclassOf<T>;

        TClassViewIterator() = default;

        explicit TClassViewIterator(const TSharedRef<TArray<TSubclassOf<T>>> &Objects,
                                    SizeType Index = 0) : Objects(Objects), Index(Index) {
        }

        bool operator==(const TClassViewIterator &Other) const {
            check(Objects == Other.Objects)
            return Index == Other.Index;
        }

        TSubclassOf<T> &operator *() const {
            check(Objects.IsValid() && Objects->IsValidIndex(Index))
            return (*Objects)[Index];
        }

        TClassViewIterator &operator++() {
            ++Index;
            return *this;
        }

        TClassViewIterator operator++(int) {
            TClassViewIterator Temp = *this;
            ++*this;
            return Temp;
        }

    private:
        TSharedPtr<TArray<TSubclassOf<T>>> Objects;
        SizeType Index = INDEX_NONE;
    };

    template <typename T = UObject>
        requires std::derived_from<T, UObject>
    class TClassView {
    public:
        explicit TClassView(bool bIncludeAbstract = false, bool bIncludeDerivedClasses = true,
                            EObjectFlags ExclusionFlags = RF_ClassDefaultObject,
                            EInternalObjectFlags ExclusionInternalFlags = EInternalObjectFlags::None) {
            auto ClassCheck = [this, bIncludeAbstract](UObject *Object) {
                auto AsClass = static_cast<UClass *>(Object);
                if (!AsClass->IsChildOf<T>() || (!bIncludeAbstract && AsClass->HasAnyClassFlags(CLASS_Abstract)) || !IsInstantiableClass(AsClass)) {
                    return;
                }
                
                ClassArray->Emplace(AsClass);
            };
            ForEachObjectOfClass(UClass::StaticClass(), ClassCheck, bIncludeDerivedClasses, ExclusionFlags, ExclusionInternalFlags);
        }

        explicit TClassView(TArray<TSubclassOf<T>> &&ClassArray) : ClassArray(
            MakeShared<TArray<TSubclassOf<T>>>(std::move(ClassArray))) {
        }

        TClassViewIterator<T> begin() const {
            return TClassViewIterator<T>(ClassArray);
        }

        TClassViewIterator<T> end() const {
            return TClassViewIterator<T>(ClassArray, ClassArray->Num());
        }

    private:
        TSharedRef<TArray<TSubclassOf<T>>> ClassArray = MakeShared<TArray<TSubclassOf<T>>>();
    };
}

template <typename T>
    requires std::derived_from<T, UObject>
constexpr inline bool ranges::enable_view<UE::Ranges::TClassView<T>> = true;

static_assert(ranges::viewable_range<UE::Ranges::TClassView<UObject>>);