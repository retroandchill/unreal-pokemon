using System.Diagnostics.CodeAnalysis;
using UnrealSharp;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameplayTags;

namespace GameDataAccessTools.Core;

public interface IGameDataRepository<T> where T : struct, IGameDataEntry, MarshalledStruct<T>
{
    UScriptStruct EntryStruct { get; }

    IReadOnlyList<T> Entries { get; }

    int NumEntries { get; }

    T GetEntry(int key);

    bool TryGetEntry(int index, [NotNullWhen(true)] out T? entry);
}
