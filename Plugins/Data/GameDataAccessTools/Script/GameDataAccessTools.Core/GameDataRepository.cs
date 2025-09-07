using System.Diagnostics.CodeAnalysis;
using GameDataAccessTools.Core.Views;
using UnrealSharp;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameplayTags;

namespace GameDataAccessTools.Core;

public interface IGameDataRepository<T>
    where T : struct, IGameDataEntry, MarshalledStruct<T>
{
    UScriptStruct EntryStruct { get; }

    ArrayView<StructView<T>> Entries { get; }

    int NumEntries { get; }

    bool IsValidEntry(FName id);

    StructView<T> GetEntry(FName id);

    StructView<T> GetEntry(int index);

    bool TryGetEntry(FName id, out StructView<T> entry);

    bool TryGetEntry(int index, out StructView<T> entry);
}

public interface IStaticGameDataRepository<T> : IGameDataRepository<T>
    where T : struct, IGameDataEntry, MarshalledStruct<T>
{
    void RegisterEntry(T entry);

    bool TryRegisterEntry(T entry);

    void UnregisterEntry(FName id);

    bool TryUnregisterEntry(FName id);
}
