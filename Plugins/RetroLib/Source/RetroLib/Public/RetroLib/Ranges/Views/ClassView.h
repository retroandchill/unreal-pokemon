#pragma once

#ifdef __UNREAL__
#if !RETROLIB_WITH_MODULES
#include <UObject/UObjectIterator.h>
#endif
#include "RetroLib/Concepts/Interfaces.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Ranges {
	
	RETROLIB_EXPORT template <typename T>
		requires std::derived_from<T, UObject> || UnrealInterface<T>
	class TClassView {
		struct FIterator {
			using value_type = std::conditional_t<std::derived_from<T, UObject>, TSubclassOf<T>, UClass*>;
			using difference_type = std::ptrdiff_t;

			FIterator() {
				if constexpr (UnrealInterface<T>) {
					while (Source && !Source->ImplementsInterface(typename T::UClassType::StaticClass())) {
						++Source;
					}
				} else {
					while (Source && !Source->IsChildOf<T>()) {
						++Source;
					}
				}
			}

			FIterator(const FIterator&) = delete;
			FIterator(FIterator&&) = default;

			~FIterator() = default;
			
			FIterator& operator=(const FIterator&) = delete;
			FIterator& operator=(FIterator&&) = default;

			TSubclassOf<T> operator*() const requires std::derived_from<T, UObject> {
				return *Source;
			}

			UClass* operator*() const requires UnrealInterface<T> {
				return *Source;
			}

			UClass* operator->() const {
				return *Source;
			}

			bool operator==(const std::default_sentinel_t&) const {
				return !Source;
			}

			FIterator &operator++() requires std::derived_from<T, UObject> {
				while (true) {
					++Source;
					if (!Source || Source->IsChildOf<T>()) {
						break;
					}
				}
				
				return *this;
			}

			FIterator &operator++() requires UnrealInterface<T> {
				while (true) {
					++Source;
					if (!Source || Source->ImplementsInterface(typename T::UClassType::StaticClass())) {
						break;
					}
				}
				
				return *this;
			}

			void operator++(int) {
				++*this;
			}

		private:
			TObjectIterator<UClass> Source;
		};
		
	public:
		TClassView() = default;

		FIterator begin() const {
			return FIterator();
		}

		std::default_sentinel_t end() const {
			return std::default_sentinel_t();
		}

		
	};
}
#endif