using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Mappers;
using Pokemon.Editor.Serialization.Model.Pbs;

namespace Pokemon.Editor.Serialization.Pbs.Serializers;

public sealed class TypePbsSerializer : IPbsSerializer<FType>
{
    public Type Type => typeof(FType);

    public void Serialize(IEnumerable<FType> model, TextWriter writer)
    {
        PbsCompiler.WritePbs(model.Select(x => x.ToTypeInfo()), writer);
    }

    public IEnumerable<FType> Deserialize(TextReader reader)
    {
        return PbsCompiler
            .CompilePbsFile<TypeInfo>(reader)
            .Select(x => x.Value)
            .Select(x => x.ToType());
    }
}
