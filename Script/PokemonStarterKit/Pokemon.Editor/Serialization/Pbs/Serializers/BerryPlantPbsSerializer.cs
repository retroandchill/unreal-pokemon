using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Mappers;
using Pokemon.Editor.Serialization.Model.Pbs;

namespace Pokemon.Editor.Serialization.Pbs.Serializers;

public sealed class BerryPlantPbsSerializer : IPbsSerializer<FBerryPlant>
{
    public Type Type => typeof(FBerryPlant);

    public void Serialize(IEnumerable<FBerryPlant> model, TextWriter writer)
    {
        PbsCompiler.WritePbs(model.Select(x => x.ToBerryPlantInfo()), writer);
    }

    public IEnumerable<FBerryPlant> Deserialize(TextReader reader)
    {
        return PbsCompiler
            .CompilePbsFile<BerryPlantInfo>(reader)
            .Select(x => x.Value)
            .Select(x => x.ToBerryPlant());
    }
}
