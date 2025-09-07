using GameDataAccessTools.Core.Views;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;

namespace GameDataAccessTools.Core;

[UStruct]
public readonly partial record struct FDataHandleEntry(FName ID, FText DisplayName)
{
    public FDataHandleEntry(FName id)
        : this(id, id.ToString()) { }
}

public interface IDataHandle : IEquatable<FName>
{
    static abstract UScriptStruct EntryStruct { get; }

    static abstract IEnumerable<FDataHandleEntry> Entries { get; }

    FName ID { get; }

    bool IsValid { get; }
}

public interface IDataHandle<T> : IDataHandle
    where T : struct, IGameDataEntry, MarshalledStruct<T>
{
    StructView<T> Entry { get; }
}
