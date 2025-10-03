using Pokemon.Battle.Components;
using Pokemon.Battle.Entities;
using UnrealSharp;

namespace Pokemon.Battle.Effects;

/// <summary>
/// Provides extension methods for handling and managing effects within battle components or effect entities.
/// This includes operations such as checking if an effect exists, retrieving effects, setting effects,
/// and resetting effects.
/// </summary>
public static class EffectExtensions
{
    /// <summary>
    /// Determines whether the specified effect is applied to the component.
    /// </summary>
    /// <typeparam name="T">The type of the effect data, which must be a struct implementing <see cref="MarshalledStruct{T}"/>.</typeparam>
    /// <param name="component">The battle effect component in which to check for the effect.</param>
    /// <param name="effect">The effect handle that identifies the effect to check for.</param>
    /// <returns>True if the component contains the specified effect; otherwise, false.</returns>
    public static bool HasEffect<T>(this UBattleEffectComponent component, EffectHandle<T> effect)
        where T : struct, MarshalledStruct<T>
    {
        return component.HasEffect(effect.Tag);
    }

    /// <summary>
    /// Determines whether the specified effect exists within the effect entity.
    /// </summary>
    /// <typeparam name="T">The type of the effect data, which must be a struct implementing <see cref="MarshalledStruct{T}"/>.</typeparam>
    /// <param name="entity">The effect entity to check for the specified effect.</param>
    /// <param name="effect">The handle identifying the effect to verify.</param>
    /// <returns>True if the specified effect exists within the entity; otherwise, false.</returns>
    public static bool HasEffect<T>(this IEffectEntity entity, EffectHandle<T> effect)
        where T : struct, MarshalledStruct<T>
    {
        return entity.EffectComponent.HasEffect(effect);
    }

    /// <summary>
    /// Retrieves the effect data of the specified type associated with the given effect handle in the battle effect component.
    /// </summary>
    /// <typeparam name="T">The type of the effect data, which must be a struct implementing <see cref="MarshalledStruct{T}"/>.</typeparam>
    /// <param name="component">The battle effect component from which to retrieve the effect data.</param>
    /// <param name="effect">The effect handle that identifies the effect to retrieve.</param>
    /// <returns>The effect data of the specified type.</returns>
    public static T GetEffect<T>(this UBattleEffectComponent component, EffectHandle<T> effect)
        where T : struct, MarshalledStruct<T>
    {
        return component.GetEffect<T>(effect.Tag);
    }

    /// <summary>
    /// Retrieves the effect data of the specified type from the component.
    /// </summary>
    /// <typeparam name="T">The type of the effect data, which must be a struct implementing <see cref="MarshalledStruct{T}"/>.</typeparam>
    /// <param name="entity">The battle effect entity from which to retrieve the effect.</param>
    /// <param name="effect">The effect handle that identifies the effect to retrieve.</param>
    /// <returns>The data associated with the specified effect.</returns>
    public static T GetEffect<T>(this IEffectEntity entity, EffectHandle<T> effect)
        where T : struct, MarshalledStruct<T>
    {
        return entity.EffectComponent.GetEffect(effect);
    }

    /// <summary>
    /// Retrieves the effect specified by the given handle from the component, or returns the default value if the effect is not present.
    /// </summary>
    /// <typeparam name="T">The type of the effect data, which must be a struct implementing <see cref="MarshalledStruct{T}"/>.</typeparam>
    /// <param name="component">The battle effect component to retrieve the effect from.</param>
    /// <param name="effect">The effect handle specifying the effect to retrieve, including the default value.</param>
    /// <returns>The effect data if present; otherwise, the default value provided by the effect handle.</returns>
    public static T GetEffectOrDefault<T>(this UBattleEffectComponent component, EffectHandle<T> effect)
        where T : struct, MarshalledStruct<T>
    {
        return component.GetEffectOrDefault(effect.Tag, effect.DefaultValue);
    }

    /// <summary>
    /// Retrieves the effect data associated with the specified effect handle from the component.
    /// If the effect is not found, returns the specified default value.
    /// </summary>
    /// <typeparam name="T">The type of the effect data, which must be a struct implementing <see cref="MarshalledStruct{T}"/>.</typeparam>
    /// <param name="component">The battle effect component from which to retrieve the effect.</param>
    /// <param name="effect">The effect handle identifying the effect to retrieve.</param>
    /// <param name="defaultValue">The default value to return if the effect is not found.</param>
    /// <returns>The effect data if found; otherwise, the specified default value.</returns>
    public static T GetEffectOrDefault<T>(this UBattleEffectComponent component, EffectHandle<T> effect, T defaultValue)
        where T : struct, MarshalledStruct<T>
    {
        return component.GetEffectOrDefault(effect.Tag, defaultValue);
    }

    /// <summary>
    /// Retrieves the specified effect from the entity's effect component, or returns the default value of the effect type if it does not exist.
    /// </summary>
    /// <typeparam name="T">The type of the effect data, which must be a struct implementing <see cref="MarshalledStruct{T}"/>.</typeparam>
    /// <param name="entity">The entity containing the effect component where the effect is stored.</param>
    /// <param name="effect">The effect handle that identifies the effect to retrieve.</param>
    /// <returns>The effect data if it exists; otherwise, the default value for the effect type.</returns>
    public static T GetEffectOrDefault<T>(this IEffectEntity entity, EffectHandle<T> effect)
        where T : struct, MarshalledStruct<T>
    {
        return entity.EffectComponent.GetEffectOrDefault(effect);
    }

    /// <summary>
    /// Retrieves the specified effect from the battle effect component, or a default value if the effect is not found.
    /// </summary>
    /// <typeparam name="T">The type of the effect data, which must be a struct implementing <see cref="MarshalledStruct{T}"/>.</typeparam>
    /// <param name="entity">The battle effect entity to retrieve the effect from.</param>
    /// <param name="effect">The effect handle identifying the effect to retrieve.</param>
    /// <param name="defaultValue">The default value to return if the effect is not found.</param>
    /// <returns>The effect data if it exists in the component; otherwise, the default value of type <typeparamref name="T"/>.</returns>
    public static T GetEffectOrDefault<T>(this IEffectEntity entity, EffectHandle<T> effect, T defaultValue)
        where T : struct, MarshalledStruct<T>
    {
        return entity.EffectComponent.GetEffectOrDefault(effect, defaultValue);
    }

    /// <summary>
    /// Attempts to retrieve the specified effect data from the battle effect component.
    /// </summary>
    /// <typeparam name="T">The type of the effect data, which must be a struct implementing <see cref="MarshalledStruct{T}"/>.</typeparam>
    /// <param name="component">The battle effect component from which to retrieve the effect.</param>
    /// <param name="effect">The effect handle that specifies the effect to retrieve.</param>
    /// <param name="data">When this method returns, contains the retrieved effect data if the operation succeeds; otherwise, the default value of <typeparamref name="T"/>.</param>
    /// <returns>True if the effect is successfully retrieved; otherwise, false.</returns>
    public static bool TryGetEffect<T>(this UBattleEffectComponent component, EffectHandle<T> effect, out T data)
        where T : struct, MarshalledStruct<T>
    {
        return component.TryGetEffect(effect.Tag, out data);
    }

    /// <summary>
    /// Attempts to retrieve the specified effect from the entity or its associated component.
    /// </summary>
    /// <typeparam name="T">The type of the effect data, which must be a struct implementing <see cref="MarshalledStruct{T}"/>.</typeparam>
    /// <param name="entity">The entity from which to retrieve the effect.</param>
    /// <param name="effect">The effect handle that identifies the effect to retrieve.</param>
    /// <param name="data">
    /// When this method returns, contains the effect data if the retrieval is successful;
    /// otherwise, contains the default value for the type <typeparamref name="T"/>.
    /// </param>
    /// <returns>
    /// True if the effect is successfully retrieved from the entity or its component;
    /// otherwise, false.
    /// </returns>
    public static bool TryGetEffect<T>(this IEffectEntity entity, EffectHandle<T> effect, out T data)
        where T : struct, MarshalledStruct<T>
    {
        return entity.EffectComponent.TryGetEffect(effect, out data);
    }

    /// <summary>
    /// Applies the specified effect to the component or updates the effect if it already exists.
    /// </summary>
    /// <typeparam name="T">The type of the effect data, which must be a struct implementing <see cref="MarshalledStruct{T}"/>.</typeparam>
    /// <param name="component">The battle effect component to which the effect should be applied or updated.</param>
    /// <param name="effect">The effect handle that identifies the effect to be applied.</param>
    /// <param name="data">The effect data to be applied or used to update the existing effect.</param>
    /// <param name="allowTypeChange">Indicates whether the type of the effect can be changed if a type mismatch occurs.</param>
    /// <returns>True if the effect was successfully applied or updated; otherwise, false.</returns>
    public static bool SetEffect<T>(
        this UBattleEffectComponent component,
        EffectHandle<T> effect,
        T data,
        bool allowTypeChange = false
    )
        where T : struct, MarshalledStruct<T>
    {
        return component.SetEffect(effect.Tag, data, allowTypeChange);
    }

    /// <summary>
    /// Applies or updates the specified effect on the battle effect component with new data.
    /// </summary>
    /// <typeparam name="T">The type of the effect data, which must be a struct implementing <see cref="MarshalledStruct{T}"/>.</typeparam>
    /// <param name="component">The battle effect component to which the effect will be applied or updated.</param>
    /// <param name="effect">The effect handle that identifies the effect to set or update.</param>
    /// <param name="updateFunc">The transformation function used to update the value.</param>
    /// <param name="allowTypeChange">Specifies whether changing the effect's type is allowed when applying or updating the effect.</param>
    /// <returns>True if the effect was successfully applied or updated; otherwise, false.</returns>
    public static bool SetEffect<T>(
        this UBattleEffectComponent component,
        EffectHandle<T> effect,
        Func<T, T> updateFunc,
        bool allowTypeChange = false
    )
        where T : struct, MarshalledStruct<T>
    {
        return component.SetEffect(
            effect,
            component.TryGetEffect(effect, out var currentData)
                ? updateFunc(currentData)
                : updateFunc(effect.DefaultValue),
            allowTypeChange
        );
    }

    /// <summary>
    /// Sets the specified effect on a battle effect component or entity. If the effect already exists, it can be updated or replaced.
    /// </summary>
    /// <typeparam name="T">The type of the effect data, which must be a struct implementing <see cref="MarshalledStruct{T}"/>.</typeparam>
    /// <param name="entity">The battle effect entity on which to set the effect.</param>
    /// <param name="effect">The effect handle that identifies the effect to set.</param>
    /// <param name="data">The data associated with the effect to store or update.</param>
    /// <param name="allowTypeChange">Indicates whether a change in the type of the stored effect is allowed.</param>
    /// <returns>True if the effect was successfully set or updated; otherwise, false.</returns>
    public static bool SetEffect<T>(
        this IEffectEntity entity,
        EffectHandle<T> effect,
        T data,
        bool allowTypeChange = false
    )
        where T : struct, MarshalledStruct<T>
    {
        return entity.EffectComponent.SetEffect(effect, data, allowTypeChange);
    }

    /// <summary>
    /// Sets or updates the specified effect associated with the entity using a provided function.
    /// </summary>
    /// <typeparam name="T">The type of the effect data, which must be a struct implementing <see cref="MarshalledStruct{T}"/>.</typeparam>
    /// <param name="entity">The effect entity to which the effect should be applied or updated.</param>
    /// <param name="effect">The effect handle representing the effect to modify.</param>
    /// <param name="updateFunc">A function that defines how to update the current effect data.</param>
    /// <param name="allowTypeChange">Specifies whether to allow a type change when setting the effect. The default value is false.</param>
    /// <returns>True if the effect was successfully set or updated; otherwise, false.</returns>
    public static bool SetEffect<T>(
        this IEffectEntity entity,
        EffectHandle<T> effect,
        Func<T, T> updateFunc,
        bool allowTypeChange = false
    )
        where T : struct, MarshalledStruct<T>
    {
        return entity.EffectComponent.SetEffect(effect, updateFunc, allowTypeChange);
    }

    /// <summary>
    /// Resets the specified effect on the battle effect component to its default value.
    /// </summary>
    /// <typeparam name="T">The type of the effect data, which must be a struct implementing <see cref="MarshalledStruct{T}"/>.</typeparam>
    /// <param name="component">The battle effect component to reset the effect on.</param>
    /// <param name="effect">The effect handle that identifies the effect to reset.</param>
    public static void ResetEffect<T>(this UBattleEffectComponent component, EffectHandle<T> effect)
        where T : struct, MarshalledStruct<T>
    {
        component.SetEffect(effect.Tag, effect.DefaultValue);
    }

    /// <summary>
    /// Resets the specified effect in the component, removing its data and any associated state.
    /// </summary>
    /// <typeparam name="T">The type of the effect data, which must be a struct implementing <see cref="MarshalledStruct{T}"/>.</typeparam>
    /// <param name="entity">The battle effect entity where the specified effect will be reset.</param>
    /// <param name="effect">The effect handle that identifies the effect to reset.</param>
    public static void ResetEffect<T>(this IEffectEntity entity, EffectHandle<T> effect)
        where T : struct, MarshalledStruct<T>
    {
        entity.EffectComponent.ResetEffect(effect);
    }
}
