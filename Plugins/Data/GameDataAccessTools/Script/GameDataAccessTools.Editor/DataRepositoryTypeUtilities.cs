using System.Reflection;
using GameDataAccessTools.Core;
using UnrealSharp;
using UnrealSharp.GameDataAccessTools;

namespace GameDataAccessTools.Editor;

/// <summary>
/// Provides utility methods for working with data repository type structures
/// and their associated data handle entries in the application.
/// </summary>
public static class DataRepositoryTypeUtilities
{
    /// <summary>
    /// Determines whether the specified type implements the <see cref="IDataHandle"/> interface.
    /// </summary>
    /// <param name="managedStructType">The type to evaluate for implementing the <see cref="IDataHandle"/> interface.</param>
    /// <returns>True if the specified type implements the <see cref="IDataHandle"/> interface; otherwise, false.</returns>
    public static bool IsGameDataHandle(this Type managedStructType)
    {
        return managedStructType.GetInterfaces().Any(i => i == typeof(IDataHandle));
    }

    /// <summary>
    /// Retrieves a collection of <see cref="FDataHandleEntry"/> instances associated with the specified type,
    /// which must implement the <see cref="IDataHandle"/> interface.
    /// </summary>
    /// <param name="managedStructType">The type from which to retrieve data handle entries. The type must implement <see cref="IDataHandle"/>.</param>
    /// <returns>A collection of <see cref="FDataHandleEntry"/> instances associated with the provided type.</returns>
    public static IEnumerable<FDataHandleEntry> GetDataHandleEntries(this Type managedStructType)
    {
        var genericMethod = typeof(DataRepositoryTypeUtilities).GetMethod(
            nameof(GetDataHandleEntriesInternal),
            BindingFlags.NonPublic | BindingFlags.Static
        );
        ArgumentNullException.ThrowIfNull(genericMethod);

        var instancedGeneric = genericMethod.MakeGenericMethod(managedStructType);
        ArgumentNullException.ThrowIfNull(instancedGeneric);

        try
        {
            return (IEnumerable<FDataHandleEntry>)instancedGeneric.Invoke(null, null)!;
        }
        catch (TargetInvocationException e)
        {
            if (e.InnerException is not null)
            {
                throw e.InnerException;
            }

            throw new InvalidOperationException("Failed to invoke GetDataHandleEntries", e);
        }
    }

    private static IEnumerable<FDataHandleEntry> GetDataHandleEntriesInternal<T>()
        where T : IDataHandle
    {
        return T.Entries;
    }
}
