using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;

namespace GameAccessTools.SourceGenerator.Utilities;

public class MemberAccessRewriter(
    SemanticModel semanticModel,
    INamedTypeSymbol containingType,
    string? propertyName = null
) : CSharpSyntaxRewriter
{
    public override SyntaxNode VisitBlock(BlockSyntax node)
    {
        // Process each statement in the block
        var newStatements = node.Statements.Select(s => (StatementSyntax)Visit(s));
        return node.WithStatements(SyntaxFactory.List(newStatements));
    }

    public override SyntaxNode VisitReturnStatement(ReturnStatementSyntax node)
    {
        if (node.Expression == null)
            return node;

        var rewrittenExpression = (ExpressionSyntax)Visit(node.Expression);
        return node.WithExpression(rewrittenExpression);
    }

    public override SyntaxNode VisitGenericName(GenericNameSyntax node)
    {
        // Visit type arguments first
        var newTypeArgs = node.TypeArgumentList.Arguments.Select(Visit).OfType<TypeSyntax>();
        var newTypeArgList = node.TypeArgumentList.WithArguments(
            SyntaxFactory.SeparatedList(newTypeArgs)
        );

        return node.WithTypeArgumentList(newTypeArgList);
    }

    public override SyntaxNode VisitInvocationExpression(InvocationExpressionSyntax node)
    {
        // First visit any arguments
        var arguments = node.ArgumentList.Arguments.Select(a =>
            a.WithExpression((ExpressionSyntax)Visit(a.Expression))
        );

        var newArgumentList = node.ArgumentList.WithArguments(
            SyntaxFactory.SeparatedList(arguments)
        );

        SimpleNameSyntax? simpleName = node.Expression switch
        {
            IdentifierNameSyntax identifier => identifier,
            GenericNameSyntax generic => generic,
            _ => null,
        };

        if (simpleName == null)
        {
            var visitedExpression = (ExpressionSyntax)Visit(node.Expression)!;
            return node.WithExpression(visitedExpression).WithArgumentList(newArgumentList);
        }

        // Get symbol info for the method being called
        if (
            semanticModel.GetSymbolInfo(node.Expression).Symbol is not IMethodSymbol methodSymbol
            || methodSymbol.IsStatic
            || !SymbolEqualityComparer.Default.Equals(methodSymbol.ContainingType, containingType)
        )
        {
            return node.WithArgumentList(newArgumentList);
        }

        // Add structView prefix
        return node.WithExpression(
                SyntaxFactory.MemberAccessExpression(
                    SyntaxKind.SimpleMemberAccessExpression,
                    SyntaxFactory.IdentifierName("structView"),
                    simpleName
                )
            )
            .WithArgumentList(newArgumentList);
    }

    public override SyntaxNode VisitMemberAccessExpression(MemberAccessExpressionSyntax node)
    {
        // First visit and transform the expression part (left side)
        var newExpression = Visit(node.Expression);

        // Not a member of our struct, preserve the original structure
        if (!IsMemberOfOurStruct(node.Expression))
            return node.Update((ExpressionSyntax)newExpression, node.OperatorToken, node.Name);

        // If this is a member of our struct, prefix with structView
        if (newExpression is MemberAccessExpressionSyntax syntax)
        {
            // Already transformed, just keep going
            return SyntaxFactory.MemberAccessExpression(
                SyntaxKind.SimpleMemberAccessExpression,
                syntax,
                node.Name
            );
        }

        // Need to prefix with structView
        return SyntaxFactory.MemberAccessExpression(
            SyntaxKind.SimpleMemberAccessExpression,
            SyntaxFactory.MemberAccessExpression(
                SyntaxKind.SimpleMemberAccessExpression,
                SyntaxFactory.IdentifierName("structView"),
                (SimpleNameSyntax)node.Expression
            ),
            node.Name
        );
    }

    public override SyntaxNode VisitIdentifierName(IdentifierNameSyntax node)
    {
        // Skip if we're already part of a qualified access
        if (node.Parent is MemberAccessExpressionSyntax { Expression: IdentifierNameSyntax })
            return node;

        // Skip if this is a parameter, type parameter, or local variable
        if (IsNotTransformable(node))
            return node;

        // Check if standalone identifier is a member
        if (IsMemberOfOurStruct(node))
        {
            return SyntaxFactory.MemberAccessExpression(
                SyntaxKind.SimpleMemberAccessExpression,
                SyntaxFactory.IdentifierName("structView"),
                node
            );
        }

        return node;
    }

    public override SyntaxNode? VisitFieldExpression(FieldExpressionSyntax node)
    {
        if (node.Token.IsKind(SyntaxKind.FieldKeyword) && propertyName is not null)
        {
            return SyntaxFactory.MemberAccessExpression(
                SyntaxKind.SimpleMemberAccessExpression,
                SyntaxFactory.IdentifierName("structView"),
                SyntaxFactory.IdentifierName($"{propertyName}_BackingField")
            );
        }

        return base.VisitFieldExpression(node);
    }

    private bool IsMemberOfOurStruct(ExpressionSyntax expression)
    {
        var symbolInfo = semanticModel.GetSymbolInfo(expression);
        var symbol = symbolInfo.Symbol;

        // For properties, fields, and methods
        if (
            symbol?.ContainingType != null
            && SymbolEqualityComparer.Default.Equals(symbol.ContainingType, containingType)
        )
        {
            return symbol switch
            {
                IPropertySymbol => true,
                IFieldSymbol => true,
                IMethodSymbol { IsStatic: false } => true,
                _ => false,
            };
        }

        return false;
    }

    private bool IsNotTransformable(IdentifierNameSyntax node)
    {
        var symbol = semanticModel.GetSymbolInfo(node).Symbol;
        return symbol is IParameterSymbol or ITypeParameterSymbol or ILocalSymbol;
    }
}
