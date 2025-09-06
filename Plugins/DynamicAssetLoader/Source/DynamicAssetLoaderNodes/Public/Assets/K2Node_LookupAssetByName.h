// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_DynamicAssetLoadBase.h"

#include "K2Node_LookupAssetByName.generated.h"

/**
 * @class UK2Node_LookupAssetByName
 *
 * A class responsible for loading an asset by its name within the Unreal Engine 5 editor.
 * This K2 node facilitates the loading of assets dynamically within the Blueprint scripting environment.
 *
 * The primary use case for UK2Node_LookupAssetByName is to provide a way to load assets at runtime
 * by specifying the asset's name as a string. This can be useful in scenarios where assets
 * are managed or referenced dynamically during game execution.
 *
 * Features:
 * - Allows specification of asset names as strings to load various asset types dynamically.
 * - Designed for use within Unreal Engine Blueprints for easy integration and usability.
 *
 * This class interacts with Unreal Engine's asset management and handles
 * operations to ensure blocking asset loading. It is part of the K2 (Kismet 2) node system,
 * providing functionality that can be integrated directly into Blueprint graphs.
 *
 * Example: (Do not include)
 * - Load a texture or mesh by specifying its name within a Blueprint.
 *
 * Note that proper error handling should be considered when using this node, as attempting
 * to load an asset with an invalid or incorrect name could result in runtime errors or null
 * references.
 */
UCLASS()
class DYNAMICASSETLOADERNODES_API UK2Node_LookupAssetByName : public UK2Node_DynamicAssetLoadBase
{
    GENERATED_BODY()

  public:
    UK2Node_LookupAssetByName();

  protected:
    FText GetNodeTitleFormat() const override;
    UEdGraphPin *CreateResultsPin(const FAssetClassType &AssetClass) override;
    FName GetLoadFunctionName() const override;
};
