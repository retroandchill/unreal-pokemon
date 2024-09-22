// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UtilityNodeSubsystem.generated.h"

namespace UnrealInjector {
    /**
     * Concept used to define a Blueprint Utility Node that can be called with the given arguments
     * @param T The type of the object to pass through
     * @param A The arguments passed to the execute function
     */
    template <typename T, typename... A>
    concept ExecutableUtility = requires(T* Object, A&&... Args) {
        Object->Execute(Forward<A>(Args)...);
    };

    /**
     * Concept used to define a Blueprint Utility Node that can be invalidated
     * @param T The type of the object to pass through
     */
    template <typename T>
    concept DestructibleUtility = requires(T* Object) {
        Object->Destruct();
    };
    
    /**
     * Concept used to define a Blueprint Utility Node that can be called with the given arguments
     * @param T The type of the object to pass through
     * @param A The arguments passed to the execute function
     */
    template <typename T, typename... A>
    concept BlueprintUtilityNode = std::is_base_of_v<UObject, T> && ExecutableUtility<T, A...> && DestructibleUtility<T>;
}

/**
 * Subsystem used for handling the calling of async utilities 
 */
UCLASS()
class UNREALINJECTOR_API UUtilityNodeSubsystem : public UGameInstanceSubsystem {
    GENERATED_BODY()

public:
    void Initialize(FSubsystemCollectionBase& Collection);
    
    /**
     * Execute a blueprint utility node.
     * @tparam T The type of the utility object
     * @tparam A The type of the arguments plushed to the node
     * @param Args The arguments that were passed through
     */
    template <typename T, typename... A>
        requires UnrealInjector::BlueprintUtilityNode<T, A...>
    void ExecuteUtilityFunction(A&&... Args) {
        auto Object = NewObject<T>(this, NodeTypes.FindChecked(T::StaticClass()));
        CreatedNodes.Add(Object);
        Object->Execute(Forward<A>(Args)...);
    }

    /**
     * Destroy a node in that has been completed.
     * @param Object The object to be removed to allowed garbage collection on
     */
    void DestroyNode(UObject* Object);

private:
    UPROPERTY()
    TMap<TSubclassOf<UObject>, TSubclassOf<UObject>> NodeTypes;

    UPROPERTY()
    TSet<TObjectPtr<UObject>> CreatedNodes;
};
