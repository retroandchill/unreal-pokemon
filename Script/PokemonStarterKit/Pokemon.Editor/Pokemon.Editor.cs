using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Mappers;
using Pokemon.Editor.Serialization.Model.Pbs;
using Pokemon.Editor.Serialization.Pbs;
using Pokemon.Editor.Serialization.Pbs.Serializers;
using UnrealSharp.Engine.Core.Modules;
using UnrealSharp.Log;
using SpeciesMapper = Pokemon.Editor.Serialization.Mappers.SpeciesMapper;

namespace Pokemon.Editor;

[CustomLog]
public static partial class LogPokemonEditor;

public class FPokemonEditorModule : IModuleInterface
{
    public void StartupModule()
    {
        PbsSerializer.Initialize();

        PbsSerializer
            .Instance.RegisterSerializer(
                new MappingPbsSerializer<TypeInfo, FType>(x => x.ToType(), x => x.ToTypeInfo())
            )
            .RegisterSerializer(
                new MappingPbsSerializer<AbilityInfo, FAbility>(
                    x => x.ToAbility(),
                    x => x.ToAbilityInfo()
                )
            )
            .RegisterSerializer(
                new MappingPbsSerializer<MoveInfo, FMove>(x => x.ToMove(), x => x.ToMoveInfo())
            )
            .RegisterSerializer(
                new MappingPbsSerializer<ItemInfo, FItem>(x => x.ToItem(), x => x.ToItemInfo())
            )
            .RegisterSerializer(
                new MappingPbsSerializer<BerryPlantInfo, FBerryPlant>(
                    x => x.ToBerryPlant(),
                    x => x.ToBerryPlantInfo()
                )
            )
            .RegisterSerializer(
                new MappingPbsSerializer<SpeciesInfo, FSpecies>(
                    x => SpeciesMapper.ToSpecies(x),
                    x => SpeciesMapper.ToSpeciesInfo(x)
                )
            )
            .RegisterSerializer(
                new MappingPbsSerializer<TrainerTypeInfo, FTrainerType>(
                    x => x.ToTrainerType(),
                    x => x.ToTrainerTypeInfo()
                )
            );
    }

    public void ShutdownModule()
    {
        PbsSerializer.Shutdown();

        PbsSerializer
            .Instance.RemoveSerializer<FType>()
            .RemoveSerializer<FAbility>()
            .RemoveSerializer<FMove>()
            .RemoveSerializer<FItem>()
            .RemoveSerializer<FBerryPlant>()
            .RemoveSerializer<FSpecies>()
            .RemoveSerializer<FTrainerType>();
    }
}
