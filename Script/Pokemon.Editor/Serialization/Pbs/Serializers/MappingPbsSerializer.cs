using Retro.ReadOnlyParams.Annotations;

namespace Pokemon.Editor.Serialization.Pbs.Serializers;

public class MappingPbsSerializer<TModel, TOutput>([ReadOnly] Func<TModel, TOutput> modelToOutput,
                                                   [ReadOnly] Func<TOutput, TModel> outputToModel) : IPbsSerializer<TOutput>
{
    
    public void Serialize(IEnumerable<TOutput> model, TextWriter writer)
    {
        PbsCompiler.WritePbs(model.Select(outputToModel), writer);
    }

    public IEnumerable<TOutput> Deserialize(TextReader reader)
    {
        return PbsCompiler.CompilePbsFile<TModel>(reader)
            .Select(x => x.Value)
            .Select(modelToOutput);
    }
}