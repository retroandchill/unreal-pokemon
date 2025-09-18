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

        PbsSerializer
            .Instance.RegisterSerializer(new TypePbsSerializer())
            .RegisterSerializer(new AbilityPbsSerializer())
            .RegisterSerializer(new MovePbsSerializer())
            .RegisterSerializer(new ItemPbsSerializer())
            .RegisterSerializer(new BerryPlantPbsSerializer())
            .RegisterSerializer(new SpeciesPbsSerializer())
            .RegisterSerializer(new TrainerTypePbsSerializer());
    }

    public void ShutdownModule()
    {
        PbsSerializer
            .Instance.RemoveSerializer(typeof(FType))
            .RemoveSerializer(typeof(FAbility))
            .RemoveSerializer(typeof(FMove))
            .RemoveSerializer(typeof(FItem))
            .RemoveSerializer(typeof(FBerryPlant))
            .RemoveSerializer(typeof(FSpecies))
            .RemoveSerializer(typeof(FTrainerType));
        
        PbsSerializer.Shutdown();
    }
}
