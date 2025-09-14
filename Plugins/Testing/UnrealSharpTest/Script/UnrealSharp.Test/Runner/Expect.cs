using System.Diagnostics.CodeAnalysis;
using UnrealSharp.Core;
using UnrealSharp.Log;
using UnrealSharp.Test.Interop;
using UnrealSharp.Test.Model;

namespace UnrealSharp.Test.Runner;

/// <summary>
/// The <c>Expect</c> class provides utility methods for registering expected log messages or errors
/// during automated tests, with support for specifying verbosity, matching types, pattern matching, and
/// occurrence counts. These methods enable robust validation of expected outputs for automated test scenarios.
/// </summary>
public static class Expect
{
    /// <summary>
    /// Registers an expected log message or pattern during automated testing.
    /// </summary>
    /// <param name="expectedPatternString">The expected log message pattern or string. Can be a specific string or regex pattern.</param>
    /// <param name="expectedVerbosity">Specifies the verbosity level of the expected message.</param>
    /// <param name="compareType">Defines the type of comparison to perform on the message, such as exact or contains matching.</param>
    /// <param name="occurrences">The expected number of occurrences of the log message.</param>
    /// <param name="isRegex">Indicates whether the <paramref name="expectedPatternString"/> parameter should be treated as a regex pattern.</param>
    public static void Message(
        [StringSyntax(StringSyntaxAttribute.Regex)] ReadOnlySpan<char> expectedPatternString,
        ELogVerbosity expectedVerbosity,
        EAutomationExpectedMessageFlags compareType = EAutomationExpectedMessageFlags.Contains,
        int occurrences = 1,
        bool isRegex = true
    )
    {
        unsafe
        {
            fixed (char* patternPtr = expectedPatternString)
            {
                AutomationTestExporter.CallAddExpectedMessage(
                    patternPtr,
                    expectedVerbosity,
                    compareType,
                    occurrences,
                    isRegex.ToNativeBool()
                );
            }
        }
    }

    /// <summary>
    /// Registers an expected log message or pattern during automated testing, using default verbosity and specified matching options.
    /// </summary>
    /// <param name="expectedPatternString">The expected log message pattern or string. Can be a specific string or a regex pattern.</param>
    /// <param name="compareType">Defines the type of comparison to perform on the message, such as exact or contains matching.</param>
    /// <param name="occurrences">The expected number of occurrences of the log message.</param>
    /// <param name="isRegex">Indicates whether the <paramref name="expectedPatternString"/> parameter should be treated as a regex pattern.</param>
    public static void Message(
        [StringSyntax(StringSyntaxAttribute.Regex)] ReadOnlySpan<char> expectedPatternString,
        EAutomationExpectedMessageFlags compareType = EAutomationExpectedMessageFlags.Contains,
        int occurrences = 1,
        bool isRegex = true
    )
    {
        Message(expectedPatternString, ELogVerbosity.All, compareType, occurrences, isRegex);
    }

    /// <summary>
    /// Registers an expected plain log message during automated testing. This method does not treat the message as a regex pattern.
    /// </summary>
    /// <param name="expectedMessage">The plain expected log message for validation.</param>
    /// <param name="expectedVerbosity">Specifies the verbosity level of the expected message to match.</param>
    /// <param name="compareType">Defines the type of comparison to perform on the log message, such as exact or contains matching.</param>
    /// <param name="occurrences">The expected number of occurrences of the log message.</param>
    public static void MessagePlain(
        ReadOnlySpan<char> expectedMessage,
        ELogVerbosity expectedVerbosity,
        EAutomationExpectedMessageFlags compareType = EAutomationExpectedMessageFlags.Contains,
        int occurrences = 1
    )
    {
        Message(expectedMessage, expectedVerbosity, compareType, occurrences, false);
    }

    /// <summary>
    /// Registers an expected plain log message during automated testing, focusing on exact text matches without regex processing.
    /// </summary>
    /// <param name="expectedMessage">The expected log message string. This is treated as plain text, not a regex pattern.</param>
    /// <param name="compareType">Specifies the type of comparison to perform on the message, such as exact or contains matching.</param>
    /// <param name="occurrences">The expected number of occurrences of the log message.</param>
    public static void MessagePlain(
        ReadOnlySpan<char> expectedMessage,
        EAutomationExpectedMessageFlags compareType = EAutomationExpectedMessageFlags.Contains,
        int occurrences = 1
    )
    {
        MessagePlain(expectedMessage, ELogVerbosity.All, compareType, occurrences);
    }

    /// <summary>
    /// Registers an expected error log message or pattern during automated testing.
    /// </summary>
    /// <param name="expectedPatternString">The expected error message pattern or string. Can be a specific string or regex pattern.</param>
    /// <param name="compareType">Defines the type of comparison to perform on the message, such as exact match or contains matching.</param>
    /// <param name="occurrences">The expected number of occurrences of the error log message.</param>
    /// <param name="isRegex">Indicates whether the <paramref name="expectedPatternString"/> parameter should be treated as a regex pattern.</param>
    public static void Error(
        [StringSyntax(StringSyntaxAttribute.Regex)] ReadOnlySpan<char> expectedPatternString,
        EAutomationExpectedMessageFlags compareType = EAutomationExpectedMessageFlags.Contains,
        int occurrences = 1,
        bool isRegex = true
    )
    {
        Message(expectedPatternString, ELogVerbosity.Error, compareType, occurrences, isRegex);
    }

    /// <summary>
    /// Registers an expected error message with specific matching criteria during automated testing.
    /// </summary>
    /// <param name="expectedPatternString">The expected error message string. Can be matched to the log output exactly or partially based on <paramref name="compareType"/>.</param>
    /// <param name="compareType">Defines the type of comparison used to match the error message, such as exact match or contains match.</param>
    /// <param name="occurrences">The expected number of times the error message should appear in the log output.</param>
    public static void ErrorPlain(
        ReadOnlySpan<char> expectedPatternString,
        EAutomationExpectedMessageFlags compareType = EAutomationExpectedMessageFlags.Contains,
        int occurrences = 1
    )
    {
        Error(expectedPatternString, compareType, occurrences);
    }
}
