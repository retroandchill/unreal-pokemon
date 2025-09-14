using NUnit.Framework;
using UnrealSharp.Log;
using UnrealSharp.Test.Model;
using UnrealSharp.Test.Runner;

namespace UnrealSharp.Test.Sample;

public class ExpectedLogTest
{
    [Test]
    public void TestBasicMessageExpectation()
    {
        // Arrange
        const string expectedMessage = "Test message";

        // Assert expectation before the actual log
        Expect.MessagePlain(expectedMessage);

        // Act
        LogUnrealSharpTest.Log(expectedMessage);
    }

    [Test]
    public void TestMultipleOccurrences()
    {
        // Arrange
        const string expectedMessage = "Repeated message";

        // Expect the message to appear exactly 3 times
        Expect.MessagePlain(expectedMessage, occurrences: 3);

        // Act
        LogUnrealSharpTest.Log(expectedMessage);
        LogUnrealSharpTest.Log(expectedMessage);
        LogUnrealSharpTest.Log(expectedMessage);
    }

    [Test]
    public void TestExactMessageMatching()
    {
        // Arrange
        const string expectedMessage = "Exact match required";

        // Assert expectation with exact matching
        Expect.MessagePlain(expectedMessage, EAutomationExpectedMessageFlags.Exact);

        // Act
        LogUnrealSharpTest.Log(expectedMessage);
    }

    [Test]
    public void TestErrorMessage()
    {
        // Arrange
        const string expectedError = "Critical error occurred";

        // Assert error expectation
        Expect.ErrorPlain(expectedError);

        // Act
        LogUnrealSharpTest.LogError(expectedError);
    }

    [Test]
    public void TestRegexPatternMatching()
    {
        // Assert expectation with regex pattern
        Expect.Message(@"User\s+\d+\s+logged in", isRegex: true);

        // Act
        LogUnrealSharpTest.Log("User 123 logged in");
    }

    [Test]
    public void TestSpecificVerbosityLevel()
    {
        // Arrange
        const string message = "Verbose debug information";

        // Assert expectation with specific verbosity
        Expect.MessagePlain(message, ELogVerbosity.Warning);

        // Act
        LogUnrealSharpTest.LogWarning(message);
    }

    [Test]
    public void TestPartialMessageMatching()
    {
        // Arrange
        const string expectedPartial = "important";

        // Assert expectation with contains matching
        Expect.MessagePlain(expectedPartial);

        // Act
        LogUnrealSharpTest.Log("This is an important message");
    }
}
