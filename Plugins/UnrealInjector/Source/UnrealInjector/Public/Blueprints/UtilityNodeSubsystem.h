// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintUtilityNode.h"
#include "Lookup/InjectableDependency.h"
#include "Lookup/InjectionUtilities.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "UtilityNodeSubsystem.generated.h"

namespace UnrealInjector {
    /**
     * Concept used to define a Blueprint Utility Node that can be called with the given arguments
     * @param T The type of the object to pass through
     * @param A The arguments passed to the execute function
     */
    template <typename T, typename... A>
    concept ExecutableUtility = requires(T *Object, A &&...Args) { Object->Execute(Forward<A>(Args)...); };

    /**
     * Concept used to define a Blueprint Utility Node that can be called with the given arguments
     * @param T The type of the object to pass through
     * @param A The arguments passed to the execute function
     */
    template <typename T, typename... A>
    concept BlueprintUtilityNode =
        Injectable<T> && CanInitialize<T> && std::derived_from<T, UBlueprintUtilityNode> && ExecutableUtility<T, A...>;
} // namespace UnrealInjector

/**
 * Subsystem used for handling the calling of async utilities
 */
UCLASS()
class UNREALINJECTOR_API UUtilityNodeSubsystem : public UGameInstanceSubsystem {
    GENERATED_BODY()

  public:
    /**
     * Execute a blueprint utility node.
     * @tparam T The type of the utility object
     * @tparam A The type of the arguments plushed to the node
     * @param Args The arguments that were passed through
     */
    template <typename T, typename... A>
        requires UnrealInjector::BlueprintUtilityNode<T, A...>
    void ExecuteUtilityFunction(A &&...Args) {
        auto Object = UnrealInjector::NewInjectedDependency<T>(this);
        CreatedNodes.Emplace(Object);
        Object->Execute(Forward<A>(Args)...);
    }

    /**
     * Destroy a node in that has been completed.
     * @param Object The object to be removed to allowed garbage collection on
     */
    void DestroyNode(UBlueprintUtilityNode *Object);

  private:
    UPROPERTY()
    TSet<TObjectPtr<UBlueprintUtilityNode>> CreatedNodes;
};
