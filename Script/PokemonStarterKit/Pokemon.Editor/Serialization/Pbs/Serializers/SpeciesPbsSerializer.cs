using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Mappers;
using Pokemon.Editor.Serialization.Model.Pbs;

namespace Pokemon.Editor.Serialization.Pbs.Serializers;

public sealed class SpeciesPbsSerializer : IPbsSerializer<FSpecies>
{
    public Type Type => typeof(FSpecies);

    public void Serialize(IEnumerable<FSpecies> model, TextWriter writer)
    {
        PbsCompiler.WritePbs(model.Select(x => x.ToSpeciesInfo()), writer);
    }

    public IEnumerable<FSpecies> Deserialize(TextReader reader)
    {
        return PbsCompiler
            .CompilePbsFile<SpeciesInfo>(reader)
            .Select(x => x.Value)
            .Select(x => x.ToSpecies());
    }
}
