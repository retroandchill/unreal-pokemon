using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.GameplayTags;
using UnrealSharp.RPGCore;

namespace Pokemon.Battle.Components;

/// <summary>
/// UBattleEffectComponent is a specialized component used in the battle system for managing and manipulating
/// effects associated with entities during gameplay. It supports operations for adding, retrieving, checking,
/// and resetting effects tied to gameplay tags.
/// </summary>
[UClass]
public class UBattleEffectComponent : URPGComponent
{
    [UProperty]
    private UEffectContainer EffectContainer { get; set; }

    /// <summary>
    /// Gets the number of active effects currently stored in the effect container.
    /// </summary>
    /// <remarks>
    /// This property provides a count of all effects managed by the associated EffectContainer instance.
    /// It reflects the current state of effects applied to a battler or an active side during gameplay.
    /// </remarks>
    public int EffectCount
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Effects")]
        get => EffectContainer.EffectCount;
    }

    /// <inheritdoc />
    protected override void PreInitialize()
    {
        EffectContainer = NewObject<UEffectContainer>(this);
    }

    /// <summary>
    /// Checks whether a specific effect, identified by the given gameplay tag, exists within the effect container.
    /// </summary>
    /// <param name="effectTag">The gameplay tag corresponding to the effect to be checked.</param>
    /// <returns>True if the effect exists in the effect container; otherwise, false.</returns>
    public bool HasEffect(FGameplayTag effectTag)
    {
        return EffectContainer.HasEffect(effectTag.TagName);
    }

    /// <summary>
    /// Retrieves an effect of the specified type associated with the given gameplay tag
    /// from the effect container.
    /// </summary>
    /// <typeparam name="T">
    /// The type of the effect to retrieve. Must be a struct that implements the
    /// <see cref="MarshalledStruct{T}"/> interface.
    /// </typeparam>
    /// <param name="effectTag">
    /// The gameplay tag used to identify the effect to retrieve.
    /// </param>
    /// <returns>
    /// The requested effect of type <typeparamref name="T"/> if it exists;
    /// otherwise, returns the default value of <typeparamref name="T"/>.
    /// </returns>
    public T GetEffect<T>(FGameplayTag effectTag)
        where T : struct, MarshalledStruct<T>
    {
        return EffectContainer.GetEffectData<T>(effectTag.TagName);
    }

    /// <summary>
    /// Retrieves the effect associated with a specific gameplay tag. If the effect does not exist,
    /// returns a provided default value instead.
    /// </summary>
    /// <typeparam name="T">The type of the effect to retrieve, which must be a struct and implement MarshalledStruct.</typeparam>
    /// <param name="effectTag">The tag representing the effect to look for.</param>
    /// <param name="alternative">The default value to return if the effect is not found.</param>
    /// <returns>The effect corresponding to the specified tag, or the provided default value if the effect is not found.</returns>
    public T GetEffectOrDefault<T>(FGameplayTag effectTag, T alternative = default)
        where T : struct, MarshalledStruct<T>
    {
        return EffectContainer.GetEffectDataOrDefault(effectTag.TagName, alternative);
    }

    /// <summary>
    /// Attempts to retrieve the effect data associated with the specified gameplay tag.
    /// </summary>
    /// <typeparam name="T">The type of the effect data to retrieve.</typeparam>
    /// <param name="effectTag">The gameplay tag associated with the desired effect.</param>
    /// <param name="data">
    /// When this method returns, contains the effect data of type <typeparamref name="T"/> if the operation succeeds,
    /// or the default value of <typeparamref name="T"/> if the operation fails.
    /// </param>
    /// <returns>
    /// <c>true</c> if the effect data is successfully retrieved; otherwise, <c>false</c>.
    /// </returns>
    public bool TryGetEffect<T>(FGameplayTag effectTag, out T data)
        where T : struct, MarshalledStruct<T>
    {
        return EffectContainer.TryGetEffectData(effectTag.TagName, out data);
    }

    /// <summary>
    /// Sets the effect data for the specified gameplay tag on the component.
    /// </summary>
    /// <typeparam name="T">The type of the effect data to set.</typeparam>
    /// <param name="effectTag">The gameplay tag identifying the effect.</param>
    /// <param name="data">The effect data to be assigned.</param>
    /// <param name="allowTypeChange">Indicates whether the type of the effect can be changed if it differs.</param>
    /// <returns>true if the effect was successfully set; otherwise, false.</returns>
    public bool SetEffect<T>(FGameplayTag effectTag, T data, bool allowTypeChange = false)
        where T : struct, MarshalledStruct<T>
    {
        return EffectContainer.SetEffectData(effectTag.TagName, data, allowTypeChange);
    }

    /// <summary>
    /// Resets the effect associated with the specified gameplay tag. This removes any
    /// stored data or state linked to the given effect tag within the effect container.
    /// </summary>
    /// <param name="effectTag">The gameplay tag representing the effect to be reset.</param>
    public void ResetEffect(FGameplayTag effectTag)
    {
        EffectContainer.ResetEffect(effectTag.TagName);
    }
}
