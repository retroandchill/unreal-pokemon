using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Mappers;
using Pokemon.Editor.Serialization.Model.Pbs;

namespace Pokemon.Editor.Serialization.Pbs.Serializers;

public sealed class TrainerTypePbsSerializer : IPbsSerializer<FTrainerType>
{
    public Type Type => typeof(FTrainerType);

    public void Serialize(IEnumerable<FTrainerType> model, TextWriter writer)
    {
        PbsCompiler.WritePbs(model.Select(x => x.ToTrainerTypeInfo()), writer);
    }

    public IEnumerable<FTrainerType> Deserialize(TextReader reader)
    {
        return PbsCompiler
            .CompilePbsFile<TrainerTypeInfo>(reader)
            .Select(x => x.Value)
            .Select(x => x.ToTrainerType());
    }
}
