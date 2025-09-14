using System.Collections;
using System.Reflection;
using NUnit.Framework;
using NUnit.Framework.Interfaces;
using NUnit.Framework.Internal;
using Retro.ReadOnlyParams.Annotations;
using UnrealSharp.Test.Attributes;

namespace UnrealSharp.Test.Model;

/// <summary>
/// Represents a placeholder for dynamically generated random data within testing.
/// This class is specifically designed to work with parameters annotated with `DynamicRandomAttribute`
/// in the context of testing frameworks.
/// </summary>
/// <param name="randomAttribute">The attribute used to generate the random data.</param>
/// <param name="parameterInfo">The parameter information for the test method.</param>
/// <param name="index">The index of the parameter within the test method.</param>
public sealed class RandomPlaceholder(
    [ReadOnly] DynamicRandomAttribute randomAttribute,
    ParameterInfo parameterInfo,
    int index
) : IDataPlaceholder
{
    /// <inheritdoc />
    public int Index { get; } = index;

    /// <inheritdoc />
    public ParameterInfo ParameterInfo { get; } = parameterInfo;

    /// <inheritdoc />
    public object?[] GetData()
    {
        var random = new Randomizer();
        var dataSource = randomAttribute.GetDataSource(ParameterInfo);
        return dataSource.CollectValues(random, randomAttribute.Distinct);
    }

    /// <inheritdoc />
    public override string ToString()
    {
        return "?";
    }
}
