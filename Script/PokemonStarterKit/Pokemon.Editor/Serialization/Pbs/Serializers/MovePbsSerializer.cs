using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Mappers;
using Pokemon.Editor.Serialization.Model.Pbs;

namespace Pokemon.Editor.Serialization.Pbs.Serializers;

public sealed class MovePbsSerializer : IPbsSerializer<FMove>
{
    public Type Type => typeof(FMove);

    public void Serialize(IEnumerable<FMove> model, TextWriter writer)
    {
        PbsCompiler.WritePbs(model.Select(x => x.ToMoveInfo()), writer);
    }

    public IEnumerable<FMove> Deserialize(TextReader reader)
    {
        return PbsCompiler
            .CompilePbsFile<MoveInfo>(reader)
            .Select(x => x.Value)
            .Select(x => x.ToMove());
    }
}
