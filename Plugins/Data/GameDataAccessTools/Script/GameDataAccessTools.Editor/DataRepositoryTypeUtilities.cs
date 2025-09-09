using System.Reflection;
using GameDataAccessTools.Core;
using UnrealSharp;
using UnrealSharp.GameDataAccessTools;

namespace GameDataAccessTools.Editor;

public static class DataRepositoryTypeUtilities
{
    public static bool IsGameDataHandle(this Type managedStructType)
    {
        return managedStructType.GetInterfaces()
            .Any(i => i == typeof(IDataHandle));
    }

    public static IEnumerable<FDataHandleEntry> GetDataHandleEntries(this Type managedStructType)
    {
        var genericMethod = typeof(DataRepositoryTypeUtilities)
            .GetMethod(nameof(GetDataHandleEntriesInternal), BindingFlags.NonPublic | BindingFlags.Static);
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

    private static IEnumerable<FDataHandleEntry> GetDataHandleEntriesInternal<T>() where T : IDataHandle
    {
        return T.Entries;
    }
}