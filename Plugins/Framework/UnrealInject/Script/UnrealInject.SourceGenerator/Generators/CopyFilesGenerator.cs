using Microsoft.CodeAnalysis;
using RhoMicro.CodeAnalysis.Generated;

namespace UnrealInject.SourceGenerator.Generators;

[Generator]
internal class CopyFilesGenerator : IIncrementalGenerator
{
    /// <inheritdoc/>
    public void Initialize(IncrementalGeneratorInitializationContext context)
    {
        IncludedFileSources.RegisterToContext(context);
    }
}
