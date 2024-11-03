// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Views/ContainerView.h"

namespace UE::Ranges {
    template <typename T>
        requires std::derived_from<T, UObject>
    class TObjectViewIterator {
    public:
        using SizeType = TArray<UObject*>::SizeType;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        
        TObjectViewIterator() = default;

        explicit TObjectViewIterator(const TSharedRef<TArray<UObject*>>& Objects, SizeType Index = 0) : Objects(Objects), Index(Index) {}
        
        bool operator==(const TObjectViewIterator& Other) const {
            check(Objects == Other.Objects)
            return Index == Other.Index;
        }
        
        T& operator * () const {
            check(Objects.IsValid() && Objects->IsValidIndex(Index))
            return *static_cast<T*>((*Objects)[Index]);
        }
        
        TObjectViewIterator& operator++() {
            ++Index;
            return *this;
        }
        TObjectViewIterator operator++(int) {
            TObjectViewIterator Temp = *this;
            ++*this;
            return Temp;
        }

    private:
        TSharedPtr<TArray<UObject*>> Objects;
        SizeType Index = INDEX_NONE;
    };

    template <typename T>
        requires std::derived_from<T, UObject>
    /**
     * @class TObjectView
     * @brief A class for visualizing and interacting with TObject instances.
     *
     * The TObjectView class provides functionalities for rendering,
     * updating, and manipulating TObject instances in a user interface.
     * It supports a variety of operations to modify the appearance and
     * behavior of the TObject during its lifecycle.
     *
     * It includes methods for setting visual properties, handling user inputs,
     * and triggering events based on interactions with the TObject.
     *
     * Users can extend this class to create custom views tailored to specific
     * types of TObjects and their unique requirements, offering flexibility
     * in how TObjects are presented and manipulated in the user interface.
     *
     * You can also leverage this class to integrate with other systems or to
     * maintain consistency and standardization of object representations
     * across different components of the software.
     */
    class TObjectView {
    public:
        explicit TObjectView(EObjectFlags AdditionalExclusionFlags = RF_ClassDefaultObject, bool bIncludeDerivedClasses = true, EInternalObjectFlags InInternalExclusionFlags = EInternalObjectFlags::None) {
            GetObjectsOfClass(T::StaticClass(), *Objects, bIncludeDerivedClasses, AdditionalExclusionFlags, GetObjectIteratorDefaultInternalExclusionFlags(InInternalExclusionFlags));
        }

        explicit TObjectView(TArray<T*>&& Array) : Objects(MakeShared<TArray<UObject*>>(std::move(Array))) {}
        
        TObjectViewIterator<T> begin() const {
            return TObjectViewIterator<T>(Objects);
        }

        TObjectViewIterator<T> end() const {
            return TObjectViewIterator<T>(Objects, Objects->Num());
        }

    private:
        TSharedRef<TArray<UObject*>> Objects = MakeShared<TArray<UObject*>>();
    };

    static_assert(ranges::forward_range<TObjectView<UObject>>);
}

template <typename T>
    requires std::derived_from<T, UObject>
constexpr inline bool ranges::enable_view<UE::Ranges::TObjectView<T>> = true;

static_assert(ranges::viewable_range<UE::Ranges::TObjectView<UObject>>);

