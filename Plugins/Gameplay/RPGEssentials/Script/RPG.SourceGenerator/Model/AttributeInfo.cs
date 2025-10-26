using JetBrains.Annotations;
using Microsoft.CodeAnalysis.CSharp.Syntax;

namespace RPG.SourceGenerator.Model;

public readonly record struct AttributeInfo(AttributeSyntax Value);
