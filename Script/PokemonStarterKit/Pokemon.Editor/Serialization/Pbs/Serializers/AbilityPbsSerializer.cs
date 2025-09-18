using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Mappers;
using Pokemon.Editor.Serialization.Model.Pbs;

namespace Pokemon.Editor.Serialization.Pbs.Serializers;

public sealed class AbilityPbsSerializer : IPbsSerializer<FAbility>
{
    public Type Type => typeof(FAbility);

    public void Serialize(IEnumerable<FAbility> model, TextWriter writer)
    {
        PbsCompiler.WritePbs(model.Select(x => x.ToAbilityInfo()), writer);
    }

    public IEnumerable<FAbility> Deserialize(TextReader reader)
    {
        return PbsCompiler
            .CompilePbsFile<AbilityInfo>(reader)
            .Select(x => x.Value)
            .Select(x => x.ToAbility());
    }
}
