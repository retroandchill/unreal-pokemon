using UnrealSharp;

namespace GameDataAccessTools.Core;

public interface IGameDataEntry
{
    FName ID { get; }

    int RowIndex { get; }
}
