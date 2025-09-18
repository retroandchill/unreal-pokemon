using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Mappers;
using Pokemon.Editor.Serialization.Model.Pbs;

namespace Pokemon.Editor.Serialization.Pbs.Serializers;

public sealed class ItemPbsSerializer : IPbsSerializer<FItem>
{
    public Type Type => typeof(FItem);

    public void Serialize(IEnumerable<FItem> model, TextWriter writer)
    {
        PbsCompiler.WritePbs(model.Select(x => x.ToItemInfo()), writer);
    }

    public IEnumerable<FItem> Deserialize(TextReader reader)
    {
        return PbsCompiler
            .CompilePbsFile<ItemInfo>(reader)
            .Select(x => x.Value)
            .Select(x => x.ToItem());
    }
}
