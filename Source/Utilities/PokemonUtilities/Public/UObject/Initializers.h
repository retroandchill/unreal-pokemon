// "Unreal Pokémon" created by Retro & Chill.
#pragma once

/**
 * Create a new UObject and run its Initialize function
 * @tparam T The type of Object we are creating
 * @tparam Args The constructor arguments being passed
 * @param Outer The object that will own this particular object
 * @param Arguments The arguments for initialize
 * @return The created object
 */
template <typename T, typename... Args>
requires std::is_base_of_v<UObject, T>
T* CreateAndInit(UObject* Outer, Args... Arguments) {
	auto Obj = NewObject<T>(Outer);
	Obj->Initialize(Arguments...);
	return Obj;
}

/**
 * Create a new UObject and run its Initialize function
 * @tparam Args The constructor arguments being passed
 * @param Class The specific class to initialize
 * @param Outer The object that will own this particular object
 * @param Arguments The arguments for initialize
 * @return The created object
 */
template <typename T, typename... Args>
requires std::is_base_of_v<UObject, T>
T* CreateAndInit(UClass* Class, UObject* Outer, Args... Arguments) {
	auto Obj = NewObject<T>(Outer, Class);
	Obj->Initialize(Arguments...);
	return Obj;
}