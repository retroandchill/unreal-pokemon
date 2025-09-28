using Microsoft.CodeAnalysis;

namespace Pokemon.SourceGenerators.Model;

public record EventMethodInfo(string EventName, string ID, IMethodSymbol Method)
{
    public string MethodName => $"{Method.ContainingType!.ToDisplayString()}.{Method.Name}";
}
