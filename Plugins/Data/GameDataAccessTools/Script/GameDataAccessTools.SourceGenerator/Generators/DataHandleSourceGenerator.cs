using GameAccessTools.SourceGenerator.Attributes;
using GameAccessTools.SourceGenerator.Model;
using GameAccessTools.SourceGenerator.Properties;
using GameAccessTools.SourceGenerator.Utilities;
using HandlebarsDotNet;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Retro.SourceGeneratorUtilities.Utilities.Attributes;

namespace GameAccessTools.SourceGenerator.Generators;

[Generator]
public class DataHandleSourceGenerator : IIncrementalGenerator
{
    public void Initialize(IncrementalGeneratorInitializationContext context)
    {
        var handleProvider = context
            .SyntaxProvider.CreateSyntaxProvider(
                (n, _) => n is RecordDeclarationSyntax,
                (ctx, _) =>
                {
                    var classNode = (RecordDeclarationSyntax)ctx.Node;
                    if (
                        ctx.SemanticModel.GetDeclaredSymbol(classNode)
                        is not INamedTypeSymbol { IsValueType: true } structSymbol
                    )
                    {
                        return null;
                    }

                    return structSymbol.HasAttribute<DataHandleBaseAttribute>()
                        ? structSymbol
                        : null;
                }
            )
            .Where(s => s is not null);

        context.RegisterSourceOutput(handleProvider, Execute!);
    }

    private static void Execute(SourceProductionContext context, INamedTypeSymbol structSymbol)
    {
        var isValidType = true;
        if (
            !structSymbol
                .GetAttributes()
                .Any(a =>
                    a.AttributeClass?.ToDisplayString() == SourceContextNames.UStructAttribute
                )
        )
        {
            context.ReportDiagnostic(
                Diagnostic.Create(
                    new DiagnosticDescriptor(
                        "GDA3001",
                        "Data handle must be a UStruct",
                        "{0} must be annotated with UStruct",
                        "GameDataAccessTools",
                        DiagnosticSeverity.Error,
                        true
                    ),
                    structSymbol.Locations.First(),
                    structSymbol.Name
                )
            );
            isValidType = false;
        }

        DataHandleBaseInfo? dataHandleInfo = null;
        try
        {
            dataHandleInfo = structSymbol.GetAttributes().GetDataHandleBaseInfos().Single();
        }
        catch (InvalidOperationException)
        {
            context.ReportDiagnostic(
                Diagnostic.Create(
                    new DiagnosticDescriptor(
                        "GDA3002",
                        "DataHandleInfo may only be used once",
                        "{0} may only use one DataHandleBaseInfo attribute",
                        "GameDataAccessTools",
                        DiagnosticSeverity.Error,
                        true
                    ),
                    structSymbol.Locations.First(),
                    structSymbol.Name
                )
            );
            isValidType = false;
        }

        if (!isValidType || dataHandleInfo is null)
        {
            return;
        }

        DataHandleTemplateBase templateParams;
        try
        {
            templateParams = dataHandleInfo switch
            {
                DataHandleInfo providerRepository => new ProviderTemplateHandle(
                    structSymbol,
                    GetEntryType(providerRepository),
                    providerRepository.Type,
                    providerRepository.RepositoryName
                ),
                DataHandleInfoOneParam specificType => new ExplicitDataHandleTemplate(
                    structSymbol,
                    specificType.EntryType
                ),
                _ => throw new InvalidOperationException(
                    $"Unknown DataHandleInfo type: {dataHandleInfo.GetType().Name}"
                ),
            };
        }
        catch (InvalidOperationException e)
        {
            context.ReportDiagnostic(
                Diagnostic.Create(
                    new DiagnosticDescriptor(
                        "GDA3003",
                        "Unable to get entry type.",
                        "{0}",
                        "GameDataAccessTools",
                        DiagnosticSeverity.Error,
                        true
                    ),
                    structSymbol.Locations.First(),
                    e.Message
                )
            );
            return;
        }

        var handlebars = Handlebars.Create();
        handlebars.Configuration.TextEncoder = null;

        context.AddSource(
            $"{structSymbol.Name}.g.cs",
            handlebars.Compile(SourceTemplates.RepositoryDataHandleTemplate)(templateParams)
        );
    }

    private static ITypeSymbol GetEntryType(DataHandleInfo dataHandleInfo)
    {
        var providerType = dataHandleInfo.Type;

        if (!providerType.HasAttribute<GameDataProviderAttribute>())
        {
            throw new InvalidOperationException(
                $"Provider class must be marked with {nameof(GameDataProviderAttribute)}"
            );
        }

        var property = providerType
            .GetMembers()
            .OfType<IPropertySymbol>()
            .SingleOrDefault(s => s.Name == dataHandleInfo.RepositoryName);

        if (property is not { Type: INamedTypeSymbol typeSymbol })
        {
            throw new InvalidOperationException(
                $"Provider {dataHandleInfo.Type.Name} class does not have property named {dataHandleInfo.RepositoryName}"
            );
        }

        if (!typeSymbol.IsGameDataRepository())
        {
            throw new InvalidOperationException(
                $"Property {dataHandleInfo.RepositoryName} on {dataHandleInfo.Type.Name} is not a GameDataRepository"
            );
        }

        return typeSymbol.GetEntryType();
    }
}
