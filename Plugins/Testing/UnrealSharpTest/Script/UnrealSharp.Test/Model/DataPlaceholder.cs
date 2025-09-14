using System.Collections;
using System.Reflection;
using NUnit.Framework.Interfaces;
using UnrealSharp.Log;

namespace UnrealSharp.Test.Model;

/// <summary>
/// Represents a placeholder interface for generating or retrieving data during runtime.
/// </summary>
public interface IDataPlaceholder
{
    /// <summary>
    /// Gets the index of the current data placeholder or argument within a sequence or collection.
    /// </summary>
    /// <remarks>
    /// The <c>Index</c> property represents the position of this object in the related data processing context.
    /// This is typically used to identify and retrieve a specific element in conjunction with data-driven operations.
    /// </remarks>
    int Index { get; }

    /// <summary>
    /// Gets the metadata associated with a single parameter in a method, constructor, or similar member.
    /// </summary>
    /// <remarks>
    /// The <c>ParameterInfo</c> property provides detailed information about the parameter, including its name, type,
    /// attributes, and position within the parameter list. This is commonly used in reflection scenarios to dynamically
    /// analyze and interact with method or constructor definitions.
    /// </remarks>
    ParameterInfo ParameterInfo { get; }

    /// <summary>
    /// Retrieves an array of objects based on the underlying data generation logic.
    /// </summary>
    /// <returns>An array of objects representing the generated or retrieved data.</returns>
    object?[] GetData();
}
