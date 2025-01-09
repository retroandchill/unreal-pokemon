// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Screens/Screen.h"

namespace RPG::Menus {
    /**
     * 
     */
    template <std::derived_from<UScreen> T>
    class TScopedScreen {
    public:
        explicit TScopedScreen(T* Screen) : Screen(Screen) {
        }
        
        ~TScopedScreen() {
            if (Screen.IsValid()) {
                Screen->CloseScreen();
            }
        }

        UE_NONCOPYABLE(TScopedScreen)

        T& Get() const {
            check(Screen.IsValid())
            return *Screen;
        }

        T& operator*() const {
            return Get();
        }

        T* operator->() const {
            return &Get();
        }

        operator T*() const {
            return &Get();
        }

    private:
        TWeakObjectPtr<T> Screen;
    };
}
