using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.Engine;
using UnrealSharp.RPGCore;

namespace RPG.Core;

/// <summary>
/// Represents the result of a component search operation performed on a unit.
/// </summary>
/// <remarks>
/// This enumeration is used to indicate whether a specific component was found on the given unit.
/// </remarks>
[UEnum]
public enum EValueFindResult : byte
{
    /// <summary>
    /// The component was found.
    /// </summary>
    Found,

    /// <summary>
    /// The component was not found.
    /// </summary>
    NotFound,
}

[UClass(DisplayName = "RPG Entity Extensions")]
public partial class URPGEntityExtensions : UBlueprintFunctionLibrary
{
    /// <summary>
    /// Attempts to find a specific component of the given type on a specified unit.
    /// </summary>
    /// <param name="entity">
    /// The unit on which the component search will be performed. Must not be null.
    /// </param>
    /// <param name="componentClass">
    /// The class type of the component to search for.
    /// </param>
    /// <param name="outComponent">
    /// If the component is found, this will hold the reference to the found component. If not found, it will be set to null.
    /// </param>
    /// <returns>
    /// An <see cref="EValueFindResult"/> indicating whether the component was found or not.
    /// </returns>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Component")]
    [ExpandEnumAsExecs("ReturnValue")]
    [DeterminesOutputType(nameof(componentClass))]
    [UMetaData("DynamicOutputParam", nameof(outComponent))]
    [UMetaData("DefaultToSelf", nameof(entity))]
    public static EValueFindResult GetComponent(
        URPGEntity entity,
        TSubclassOf<URPGComponent> componentClass,
        out URPGComponent? outComponent
    )
    {
        return entity.TryGetComponent(componentClass, out outComponent)
            ? EValueFindResult.Found
            : EValueFindResult.NotFound;
    }

    /// <summary>
    /// Attempts to locate a sibling component of the specified type on the same unit as the provided component.
    /// </summary>
    /// <param name="component">
    /// The reference component whose sibling is to be found. Must not be null.
    /// </param>
    /// <param name="componentClass">
    /// The class type of the sibling component to search for.
    /// </param>
    /// <param name="outComponent">
    /// If a sibling component is found, this will hold the reference to the found component. If no matching sibling is found, it will be set to null.
    /// </param>
    /// <returns>
    /// An <see cref="EValueFindResult"/> indicating whether the sibling component was found or not.
    /// </returns>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Component")]
    [ExpandEnumAsExecs("ReturnValue")]
    [DeterminesOutputType(nameof(componentClass))]
    [UMetaData("DynamicOutputParam", nameof(outComponent))]
    [UMetaData("DefaultToSelf", nameof(component))]
    public static EValueFindResult GetSiblingComponent(
        URPGComponent component,
        TSubclassOf<URPGComponent> componentClass,
        out URPGComponent? outComponent
    )
    {
        return component.TryGetSiblingComponent(componentClass, out outComponent)
            ? EValueFindResult.Found
            : EValueFindResult.NotFound;
    }
}
