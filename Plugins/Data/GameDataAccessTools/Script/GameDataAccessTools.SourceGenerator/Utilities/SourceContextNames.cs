namespace GameAccessTools.SourceGenerator.Utilities;

public static class SourceContextNames
{
    public const string UObject = "UnrealSharp.CoreUObject.UObject";
    public const string AActor = "UnrealSharp.Engine.AActor";

    public const string IGameDataEntry = "GameDataAccessTools.Core.DataRetrieval.IGameDataEntry";
    public const string UGameDataRepository = "UnrealSharp.GameDataAccessTools.UGameDataRepository";
    public const string IGameDataRepository =
        "GameDataAccessTools.Core.DataRetrieval.IGameDataRepository<T>";

    public const string UClassAttribute = "UnrealSharp.Attributes.UClassAttribute";
    public const string UStructAttribute = "UnrealSharp.Attributes.UStructAttribute";
    public const string UPropertyAttribute = "UnrealSharp.Attributes.UPropertyAttribute";
    public const string UMetaDataAttribute = "UnrealSharp.Attributes.UMetaDataAttribute";

    public const ulong EditInlineNew = 0x00001000u;
}
