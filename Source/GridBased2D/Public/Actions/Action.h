// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Action.generated.h"

/**
 * Dispatcher for an action performable by another object.
 */
UCLASS(Blueprintable, Abstract)
class GRIDBASED2D_API UAction : public UObject {
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Perform the assigned action for the owning class.
	 * @param Owner The object that owns this action
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Actions)
	void PerformAction(UObject *Owner);
};
