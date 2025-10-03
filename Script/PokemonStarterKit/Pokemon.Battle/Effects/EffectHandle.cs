using System.Numerics;
using Pokemon.Battle.Components;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.GameplayTags;

namespace Pokemon.Battle.Effects;

/// <summary>
/// Represents a boolean effect data used in a gameplay effect system.
/// </summary>
/// <param name="Value">
/// The value of the boolean effect, representing the state of the effect.
/// </param>
/// <remarks>
/// The FBoolEffectData structure is used to encapsulate a boolean value and provides utility functions
/// such as conversion and negation to facilitate its use in a gameplay effect system.
/// Implicit conversions allow seamless use of the structure as a boolean.
/// </remarks>
[UStruct]
public readonly partial record struct FBoolEffectData(bool Value)
{
    /// <summary>
    /// Defines a custom operator to be used within the context of a specific struct or class.
    /// </summary>
    /// <remarks>
    /// Operators are defined to enable custom behavior for common operations such as addition,
    /// subtraction, logical operations, and type conversions. Custom operators allow for
    /// seamless integration of user-defined types into expressions, improving code readability
    /// and usability. Examples include implicit type conversions, arithmetic, and logical negation.
    /// </remarks>
    public static implicit operator bool(FBoolEffectData data) => data.Value;

    /// <summary>
    /// Defines a custom operator to allow a specific conversion or operation on a struct or class.
    /// </summary>
    /// <remarks>
    /// Custom operators provide functionality to perform implicit or explicit type conversions,
    /// or define behavior for common operations such as arithmetic, logical operations, and comparisons.
    /// By defining operators, user-defined types can participate in such operations in a seamless and intuitive manner.
    /// </remarks>
    public static implicit operator FBoolEffectData(bool value) => new(value);

    /// <summary>
    /// Defines a custom logical negation operator for the FBoolEffectData struct.
    /// </summary>
    /// <remarks>
    /// This operator provides the ability to use the logical negation (!) operation directly
    /// on an instance of the FBoolEffectData struct. It inverts the boolean effect value
    /// represented by the instance, enhancing usability and facilitating intuitive operations
    /// in the context of boolean effect management within a gameplay system.
    /// </remarks>
    public static bool operator !(FBoolEffectData data) => !data.Value;
}

/// <summary>
/// Represents an integer effect data used in a gameplay effect system.
/// </summary>
/// <param name="Value">
/// The value of the integer effect, representing the magnitude or state of the effect.
/// </param>
/// <remarks>
/// The FIntEffectData structure is used to encapsulate an integer value and provides utility operators
/// such as arithmetic operations to facilitate its use in a gameplay effect system.
/// Implicit conversions allow seamless use of the structure as an integer.
/// </remarks>
[UStruct]
public readonly partial record struct FIntEffectData(int Value)
    : IAdditionOperators<FIntEffectData, int, FIntEffectData>,
        ISubtractionOperators<FIntEffectData, int, FIntEffectData>,
        IMultiplyOperators<FIntEffectData, int, FIntEffectData>,
        IDivisionOperators<FIntEffectData, int, FIntEffectData>,
        IModulusOperators<FIntEffectData, int, FIntEffectData>
{
    /// <summary>
    /// Defines an operator for a struct to enable custom operations and type conversions.
    /// </summary>
    /// <remarks>
    /// Operators allow for tailored behavior of user-defined types during arithmetic, logical, or type
    /// casting operations. They enhance code clarity and usability by integrating the struct with common
    /// operations seamlessly, such as implicit and explicit type conversions, arithmetic computations,
    /// and overloads of conventional operators.
    /// </remarks>
    public static implicit operator int(FIntEffectData data) => data.Value;

    /// <summary>
    /// Defines an operator for custom handling or manipulation of the struct.
    /// </summary>
    /// <remarks>
    /// Operators provide support for overloading common operations involving user-defined types.
    /// They enable logical, arithmetic, or type-conversion functionalities with improved code
    /// readability and integration into existing workflows. Custom operators are a valuable addition
    /// in scenarios requiring concise definitions for handling data structures.
    /// </remarks>
    public static implicit operator FIntEffectData(int value) => new(value);

    /// <inheritdoc />
    public static FIntEffectData operator +(FIntEffectData data, int other) => new(data.Value + other);

    /// <inheritdoc />
    public static FIntEffectData operator -(FIntEffectData data, int other) => new(data.Value - other);

    /// <inheritdoc />
    public static FIntEffectData operator *(FIntEffectData data, int other) => new(data.Value * other);

    /// <inheritdoc />
    public static FIntEffectData operator /(FIntEffectData data, int other) => new(data.Value / other);

    /// <inheritdoc />
    public static FIntEffectData operator %(FIntEffectData data, int other) => new(data.Value % other);
}

/// <summary>
/// Represents a floating-point effect data used in a gameplay effect system.
/// </summary>
/// <param name="Value">
/// The numerical value of the floating-point effect, defining its magnitude or strength.
/// </param>
/// <remarks>
/// The FFloatEffectData structure encapsulates a single-precision floating-point number and provides utility functions
/// to perform arithmetic operations such as addition, subtraction, multiplication, division, and modulus.
/// Implicit conversions allow seamless use of the structure as a float, enabling ease of integration into gameplay systems
/// that rely on floating-point calculations.
/// </remarks>
[UStruct]
public readonly partial record struct FFloatEffectData(float Value)
    : IAdditionOperators<FFloatEffectData, float, FFloatEffectData>,
        ISubtractionOperators<FFloatEffectData, float, FFloatEffectData>,
        IMultiplyOperators<FFloatEffectData, float, FFloatEffectData>,
        IDivisionOperators<FFloatEffectData, float, FFloatEffectData>,
        IModulusOperators<FFloatEffectData, float, FFloatEffectData>
{
    /// <summary>
    /// Defines overloaded operators to provide custom behavior for mathematical and type conversion operations
    /// on the <see cref="FFloatEffectData"/> structure.
    /// </summary>
    /// <remarks>
    /// The overloaded operators enable operations such as addition, subtraction, multiplication,
    /// division, and modulus specifically for <see cref="FFloatEffectData"/> instances. This allows
    /// the structure to be seamlessly integrated into mathematical expressions, improving
    /// both usability and clarity when working with game-related effects. Implicit conversion operators
    /// are also implemented to allow for conversion between <see cref="FFloatEffectData"/> and <see cref="float"/>.
    /// </remarks>
    public static implicit operator float(FFloatEffectData data) => data.Value;

    /// <summary>
    /// Declares a custom operator for the FFloatEffectData struct.
    /// </summary>
    /// <remarks>
    /// Custom operators are used to define operations specific to the data structure,
    /// such as arithmetic operations and type conversions. This enhances the usability
    /// of the struct by allowing intuitive handling of its instances using typical operators
    /// like addition, subtraction, multiplication, division, and modulus. Such operators
    /// facilitate seamless integration into systems relying on mathematical computations
    /// and improve code readability and maintainability.
    /// </remarks>
    public static implicit operator FFloatEffectData(float value) => new(value);

    /// <inheritdoc />
    public static FFloatEffectData operator +(FFloatEffectData data, float other) => new(data.Value + other);

    /// <inheritdoc />
    public static FFloatEffectData operator -(FFloatEffectData data, float other) => new(data.Value - other);

    /// <inheritdoc />
    public static FFloatEffectData operator *(FFloatEffectData data, float other) => new(data.Value * other);

    /// <inheritdoc />
    public static FFloatEffectData operator /(FFloatEffectData data, float other) => new(data.Value / other);

    /// <inheritdoc />
    public static FFloatEffectData operator %(FFloatEffectData data, float other) => new(data.Value % other);
}

/// <summary>
/// Represents a handle for a gameplay effect within a battle system.
/// </summary>
/// <typeparam name="T">
/// The type of the data associated with the effect. This type must be a struct
/// implementing the <see cref="MarshalledStruct{T}"/> interface.
/// </typeparam>
/// <param name="Tag">The gameplay tag used to identify the effect.</param>
/// <param name="DefaultValue">The default value of the effect.</param>
/// <remarks>
/// The EffectHandle is used to uniquely identify and manage effects that are applied
/// within a battle system. The effect is identified using a gameplay tag, and it
/// optionally holds a default value of the specified effect type.
/// </remarks>
public record EffectHandle<T>(FGameplayTag Tag, T DefaultValue = default)
    where T : struct, MarshalledStruct<T>;
