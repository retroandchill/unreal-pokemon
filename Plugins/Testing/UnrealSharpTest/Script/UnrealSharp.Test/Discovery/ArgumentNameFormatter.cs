using NUnit.Framework;

namespace UnrealSharp.Test.Discovery;

/// <summary>
/// Provides utility methods for formatting argument or test case names for display purposes in test cases.
/// </summary>
public static class ArgumentNameFormatter
{
    /// <summary>
    /// Retrieves the display name of the given test case for use in test reporting or display purposes.
    /// </summary>
    /// <param name="testCase">The test case data from which to derive the display name.</param>
    /// <returns>
    /// The display name derived from the test case. If the test case's TestName property is not null,
    /// it returns that value. Otherwise, it constructs a name based on the arguments of the test case.
    /// </returns>
    public static string GetDisplayName(this TestCaseData testCase)
    {
        return testCase.TestName ?? $"({string.Join(";", testCase.Arguments.Select(GetArgumentDisplayName))})";
    }

    private static string GetArgumentDisplayName(object? argument)
    {
        return argument switch
        {
            null => "null",
            string str => $"\"{str}\"",
            char ch => $"'{ch}'",
            _ => argument.ToString()!.Replace(",", "").Replace('.', '_'),
        };
    }
}
