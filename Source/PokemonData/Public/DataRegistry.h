#pragma once

#include "CoreMinimal.h"

/**
 * Declare a registry for this class
 * @param BaseClass The name of this class
 * @param IdType The data type to use as the ID
 */
#define DECLARE_REGISTRY_CLASS(BaseClass, IdType) \
	public: \
	using Registry = DataRegistry<BaseClass, IdType>; \
	static Registry SubclassRegistry; \
	private:

/**
 * Implement the details of the registry for this class
 * @param BaseClass The name of the class
 */
#define IMPLEMENT_REGISTRY_CLASS(BaseClass) BaseClass::Registry BaseClass::SubclassRegistry

/**
 * Register this subclass to the registry of the base class
 * @param BaseClass The parent class of this class
 * @param Subclass The name of the class to be registered
 */
#define REGISTER_SUBCLASS(BaseClass, Subclass) \
	const bool _Registered_##Subclass = BaseClass::SubclassRegistry.RegisterClass<Subclass>(#Subclass)

/**
 * Template class used to handle creating a registry for a data type
 * @tparam Base The base type that the Construct method produces
 * @tparam Key The type that represents the ID of the data object
 * @tparam Args The constructor arguments taken by the derived type
 */
template<typename Base, typename Key, typename... Args>
class DataRegistry {
public:
	using FactoryFunc = TFunction<TUniquePtr<Base>(Args...)>;

	DataRegistry() = default;
	~DataRegistry() = default;

	/**
	 * Register a class using the default factory function
	 * @tparam Derived The derived type that inherits from the base type
	 * @param ID The ID of the derived class
	 * @return If the class was successfully registered
	 */
	template<typename Derived>
	bool RegisterClass(const Key &ID) {
		return RegisterFactory(ID, &ConstructDerived<Derived>);	
	}

	/**
	 * Register a factory
	 * @param ID The ID of the derived class
	 * @param Factory The function used to construct the class
	 * @return If the class was successfully registered
	 */
	bool RegisterFactory(const Key &ID, const FactoryFunc Factory) {
		FactoryMap[ID] = Factory;
		return true;
	}

	/**
	 * Construct a registered class using the ID in question. Returns null if invalid.
	 * @param ID The ID to try to construct
	 * @param Arguments The constructor arguments needed to create the type
	 * @return The constructed data object
	 */
	TUniquePtr<Base> Construct(const Key &ID, Args&&... Arguments) const {
		auto Factory = FactoryMap.Find(ID);
		if (Factory == nullptr) {
			return TUniquePtr<Base>();
		}
		
		return Factory(std::forward<Args>(Arguments)...);
	}

	/**
	 * Returns if the provided key represents a registered class
	 * @param ID The ID to check the validity of
	 * @return If the registered ID is valid
	 */
	bool IsValid(const Key& ID) const {
		return FactoryMap.Contains(ID);
	}

private:
	/**
	 * Construct a pointer to a subclass of the data class
	 * @tparam Derived The data class to construct a pointer of
	 * @param Arguments The constructor arguments for the derived type
	 * @return The constructed data object
	 */
	template<typename Derived>
	static TUniquePtr<Base> ConstructDerived(Args&&... Arguments) {
		return MakeUnique<Derived>(std::forward<Args>(Arguments)...);
	}

	/**
	 * A map that holds all of the factory functions mapped by ID
	 */
	TMap<Key, FactoryFunc> FactoryMap;
};
