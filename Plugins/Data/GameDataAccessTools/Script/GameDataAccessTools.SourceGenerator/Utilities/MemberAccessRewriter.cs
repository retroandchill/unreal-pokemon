using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;

namespace GameAccessTools.SourceGenerator.Utilities;

public class MemberAccessRewriter(
    SemanticModel semanticModel,
    INamedTypeSymbol containingType,
    string propertyName
) : CSharpSyntaxRewriter
{
    private readonly SemanticModel _semanticModel = semanticModel;
    private readonly INamedTypeSymbol _containingType = containingType;
    private readonly string _propertyName = propertyName;

    public override SyntaxNode? VisitBlock(BlockSyntax node)
    {
        // Process each statement in the block
        var newStatements = node.Statements.Select(s => (StatementSyntax)Visit(s)!);
        return node.WithStatements(SyntaxFactory.List(newStatements));
    }

    public override SyntaxNode? VisitReturnStatement(ReturnStatementSyntax node)
    {
        if (node.Expression == null)
            return node;

        var rewrittenExpression = (ExpressionSyntax)Visit(node.Expression)!;
        return node.WithExpression(rewrittenExpression);
    }

    public override SyntaxNode? VisitMemberAccessExpression(MemberAccessExpressionSyntax node)
    {
        // First visit and transform the expression part (left side)
        var newExpression = Visit(node.Expression);

        // Not a member of our struct, preserve the original structure
        if (!IsMemberOfOurStruct(node.Expression))
            return node.Update((ExpressionSyntax)newExpression!, node.OperatorToken, node.Name);

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

    public override SyntaxNode? VisitIdentifierName(IdentifierNameSyntax node)
    {
        // Skip if we're already part of a qualified access
        if (node.Parent is MemberAccessExpressionSyntax { Expression: IdentifierNameSyntax })
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
        if (node.Token.IsKind(SyntaxKind.FieldKeyword))
        {
            return SyntaxFactory.MemberAccessExpression(
                SyntaxKind.SimpleMemberAccessExpression,
                SyntaxFactory.IdentifierName("structView"),
                SyntaxFactory.IdentifierName($"{_propertyName}_BackingField")
            );
        }

        return base.VisitFieldExpression(node);
    }

    private bool IsMemberOfOurStruct(ExpressionSyntax expression)
    {
        var symbolInfo = _semanticModel.GetSymbolInfo(expression);
        var symbol = symbolInfo.Symbol;
        return symbol?.ContainingType != null
            && SymbolEqualityComparer.Default.Equals(symbol.ContainingType, _containingType);
    }
}
