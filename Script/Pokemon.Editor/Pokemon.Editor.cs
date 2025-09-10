using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Mappers;
using Pokemon.Editor.Serialization.Model.Pbs;
using Pokemon.Editor.Serialization.Pbs;
using Pokemon.Editor.Serialization.Pbs.Serializers;
using UnrealSharp.Engine.Core.Modules;
using UnrealSharp.Log;

namespace Pokemon.Editor;

[CustomLog]
public static partial class LogPokemonEditor;

public class FPokemonEditorModule : IModuleInterface
{
    public void StartupModule()
    {
        PbsSerializer.Initialize();

        PbsSerializer.Instance
            .RegisterSerializer(new MappingPbsSerializer<TypeInfo, FType>(
                x => x.ToType(),
                x => x.ToTypeInfo()));
    }

    public void ShutdownModule()
    {
        PbsSerializer.Shutdown();

        PbsSerializer.Instance.RemoveSerializer<FType>();
    }
}