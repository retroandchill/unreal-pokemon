#if RPG_ESSENTIALS_GENERATOR
using RhoMicro.CodeAnalysis;
#else
using UnrealSharp;
using UnrealSharp.RPGCore;
#endif

namespace RPG.SourceGenerator.Attributes;

/// <summary>
/// An attribute used to designate a class responsible for saving and restoring the state of a specific component type to a corresponding model.
/// </summary>
/// <typeparam name="TComponent">The type of the component for which state management functionality is defined.</typeparam>
/// <typeparam name="TModel">
/// The type of the corresponding model structure used for state serialization and deserialization.
/// Must be a value type.
/// </typeparam>
/// <remarks>
/// This attribute can only be applied to class definitions. Usage of this attribute may enforce specific requirements on the generic type parameters based on the compilation context.
/// </remarks>
#if RPG_ESSENTIALS_GENERATOR
[IncludeFile]
#endif
[AttributeUsage(AttributeTargets.Class)]
public class ComponentSaverAttribute<TComponent, TModel> : Attribute
#if RPG_ESSENTIALS_GENERATOR
    where TModel : struct
#else
    where TComponent : URPGComponent
    where TModel : struct, MarshalledStruct<TModel>
#endif
{
    /// <summary>
    /// Gets or sets the type of the logger class to be used in the attributed component saver class for logging purposes.
    /// </summary>
    /// <remarks>
    /// The specified logger class type is intended to provide logging functionality within the context of the component saver.
    /// It must be a type that implements a logging interface or behavior suitable for the application's requirements.
    /// </remarks>
    public Type? LoggerClass { get; init; }
}
