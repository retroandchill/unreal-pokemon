﻿using Microsoft.CodeAnalysis;
using RhoMicro.CodeAnalysis.Generated;

namespace GameAccessTools.SourceGenerator.Generators;

[Generator]
internal class CopyFilesGenerator : IIncrementalGenerator
{
    /// <inheritdoc/>
    public void Initialize(IncrementalGeneratorInitializationContext context)
    {
        IncludedFileSources.RegisterToContext(context);
    }
}
