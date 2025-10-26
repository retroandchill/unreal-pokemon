using System.Collections.Immutable;
using JetBrains.Annotations;
using Microsoft.CodeAnalysis;

namespace RPG.SourceGenerator.Model;

public record ComponentInfo(
    ITypeSymbol ComponentType,
    string ComponentName,
    string? InitializerName,
    ImmutableArray<UPropertyInfo> Properties,
    ImmutableArray<UFunctionInfo> Methods,
    bool IsLast = false
)
{
    public bool HasInitializer => InitializerName is not null;
}
