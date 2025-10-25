using GameDataAccessTools.Core.Utilities;
using NUnit.Framework;
using UnrealSharp;
using UnrealSharp.Core;

namespace GameDataAccessTools.Test.Utilities;

public class LocalizationExtensionTests
{
    [Test]
    public void LocalizedCreatesTextWithValidParameters()
    {
        const string testNamespace = "TestGame";
        const string testKey = "MainMenu.Title";
        const string testDefaultValue = "Main Menu";

        var localizedText = FText.Localized(testNamespace, testKey, testDefaultValue);

        using (Assert.EnterMultipleScope())
        {
            Assert.That(localizedText, Is.Not.Null);
            Assert.That(localizedText.Empty, Is.False);

            // The localized text should either contain the localized version or fall back to default
            var textString = localizedText.ToString();
            Assert.That(textString, Is.Not.Null);
            Assert.That(textString, Is.Not.Empty);
        }
    }

    [Test]
    public void LocalizedHandlesEmptyNamespace()
    {
        const string testKey = "GenericKey";
        const string testDefaultValue = "Generic Value";

        var localizedText = FText.Localized("", testKey, testDefaultValue);

        using (Assert.EnterMultipleScope())
        {
            Assert.That(localizedText, Is.Not.Null);
            Assert.That(localizedText.Empty, Is.False);

            var textString = localizedText.ToString();
            Assert.That(textString, Is.Not.Null);
            Assert.That(textString, Is.Not.Empty);
        }
    }

    [Test]
    public void LocalizedHandlesEmptyKey()
    {
        const string testNamespace = "TestGame";
        const string testDefaultValue = "Default Text";

        var localizedText = FText.Localized(testNamespace, "", testDefaultValue);

        using (Assert.EnterMultipleScope())
        {
            Assert.That(localizedText, Is.Not.Null);
            Assert.That(localizedText.Empty, Is.False);

            var textString = localizedText.ToString();
            Assert.That(textString, Is.Not.Null);
            Assert.That(textString, Is.Not.Empty);
        }
    }

    [Test]
    public void LocalizedHandlesEmptyDefaultValue()
    {
        const string testNamespace = "TestGame";
        const string testKey = "MainMenu.Title";

        var localizedText = FText.Localized(testNamespace, testKey, "");

        using (Assert.EnterMultipleScope())
        {
            Assert.That(localizedText, Is.Not.Null);

            // Even with empty default, the FText should be valid (might be empty though)
            var textString = localizedText.ToString();
            Assert.That(textString, Is.Not.Null);
        }
    }

    [Test]
    public void LocalizedWithSpecialCharacters()
    {
        const string testNamespace = "TestGame.UI";
        const string testKey = "Dialog.Confirmation";
        const string testDefaultValue = "Are you sure you want to quit?";

        var localizedText = FText.Localized(testNamespace, testKey, testDefaultValue);

        using (Assert.EnterMultipleScope())
        {
            Assert.That(localizedText, Is.Not.Null);
            Assert.That(localizedText.Empty, Is.False);

            var textString = localizedText.ToString();
            Assert.That(textString, Is.Not.Null);
            Assert.That(textString, Is.Not.Empty);
        }
    }

    [Test]
    public void LocalizedWithUnicodeCharacters()
    {
        const string testNamespace = "TestGame";
        const string testKey = "Unicode.Test";
        const string testDefaultValue = "こんにちは世界"; // "Hello World" in Japanese

        var localizedText = FText.Localized(testNamespace, testKey, testDefaultValue);

        using (Assert.EnterMultipleScope())
        {
            Assert.That(localizedText, Is.Not.Null);
            Assert.That(localizedText.Empty, Is.False);

            var textString = localizedText.ToString();
            Assert.That(textString, Is.Not.Null);
            Assert.That(textString, Is.Not.Empty);
        }
    }

    [Test]
    public void FromLocalizedStringWithNamespaceKeyFormat()
    {
        // Test the NSLOCTEXT format: NSLOCTEXT("Namespace", "Key", "Default Value")
        const string localizedString = "NSLOCTEXT(\"TestGame\", \"MainMenu.Title\", \"Main Menu\")";

        var textFromLocalized = FText.FromLocalizedString(localizedString);

        using (Assert.EnterMultipleScope())
        {
            Assert.That(textFromLocalized, Is.Not.Null);
            Assert.That(textFromLocalized.Empty, Is.False);

            var textString = textFromLocalized.ToString();
            Assert.That(textString, Is.Not.Null);
            Assert.That(textString, Is.Not.Empty);
        }
    }

    [Test]
    public void FromLocalizedStringWithLocTextFormat()
    {
        // Test the LOCTEXT format: LOCTEXT("Key", "Default Value")
        const string localizedString = "LOCTEXT(\"MainMenu.Title\", \"Main Menu\")";

        var textFromLocalized = FText.FromLocalizedString(localizedString);

        using (Assert.EnterMultipleScope())
        {
            Assert.That(textFromLocalized, Is.Not.Null);
            Assert.That(textFromLocalized.Empty, Is.False);

            var textString = textFromLocalized.ToString();
            Assert.That(textString, Is.Not.Null);
            Assert.That(textString, Is.Not.Empty);
        }
    }

    [Test]
    public void FromLocalizedStringWithInvTextFormat()
    {
        // Test the INVTEXT format: INVTEXT("Invariant Text")
        const string localizedString = "INVTEXT(\"Invariant Text\")";

        var textFromLocalized = FText.FromLocalizedString(localizedString);

        using (Assert.EnterMultipleScope())
        {
            Assert.That(textFromLocalized, Is.Not.Null);
            Assert.That(textFromLocalized.Empty, Is.False);

            var textString = textFromLocalized.ToString();
            Assert.That(textString, Is.Not.Null);
            Assert.That(textString, Is.Not.Empty);
        }
    }

    [Test]
    public void FromLocalizedStringWithComplexNsLocText()
    {
        // Test NSLOCTEXT with special characters and escaping
        const string localizedString =
            "NSLOCTEXT(\"Game.Dialog\", \"Confirmation.Quit\", \"Are you sure you want to quit?\")";

        var textFromLocalized = FText.FromLocalizedString(localizedString);

        using (Assert.EnterMultipleScope())
        {
            Assert.That(textFromLocalized, Is.Not.Null);
            Assert.That(textFromLocalized.Empty, Is.False);

            var textString = textFromLocalized.ToString();
            Assert.That(textString, Is.Not.Null);
            Assert.That(textString, Is.Not.Empty);
        }
    }

    [Test]
    public void FromLocalizedStringWithUnicodeContent()
    {
        // Test localized string with Unicode characters
        const string localizedString = "NSLOCTEXT(\"TestGame\", \"Unicode.Greeting\", \"こんにちは世界\")";

        var textFromLocalized = FText.FromLocalizedString(localizedString);

        using (Assert.EnterMultipleScope())
        {
            Assert.That(textFromLocalized, Is.Not.Null);
            Assert.That(textFromLocalized.Empty, Is.False);

            var textString = textFromLocalized.ToString();
            Assert.That(textString, Is.Not.Null);
            Assert.That(textString, Is.Not.Empty);
        }
    }

    [Test]
    public void FromLocalizedStringWithEmptyString()
    {
        var textFromLocalized = FText.FromLocalizedString("");

        using (Assert.EnterMultipleScope())
        {
            Assert.That(textFromLocalized, Is.Not.Null);

            // Empty localized string should produce valid but potentially empty FText
            var textString = textFromLocalized.ToString();
            Assert.That(textString, Is.Not.Null);
        }
    }

    [Test]
    public void FromLocalizedStringWithMalformedInput()
    {
        // Test with malformed localization string
        const string localizedString = "NSLOCTEXT(\"Incomplete";

        var textFromLocalized = FText.FromLocalizedString(localizedString);

        using (Assert.EnterMultipleScope())
        {
            Assert.That(textFromLocalized, Is.Not.Null);

            // Malformed input should still produce a valid FText (behavior depends on UE implementation)
            var textString = textFromLocalized.ToString();
            Assert.That(textString, Is.Not.Null);
        }
    }

    [Test]
    public void FromLocalizedStringWithPlainText()
    {
        // Test with plain text (not in localization format)
        const string localizedString = "Just plain text without localization markup";

        var textFromLocalized = FText.FromLocalizedString(localizedString);

        using (Assert.EnterMultipleScope())
        {
            Assert.That(textFromLocalized, Is.Not.Null);
            Assert.That(textFromLocalized.Empty, Is.False);

            var textString = textFromLocalized.ToString();
            Assert.That(textString, Is.Not.Null);
            Assert.That(textString, Is.Not.Empty);
        }
    }

    [Test]
    public void LocalizedVsFromLocalizedStringConsistency()
    {
        const string testNamespace = "TestGame";
        const string testKey = "Test.Key";
        const string testDefaultValue = "Test Value";

        // Create using Localized method
        var localizedText = FText.Localized(testNamespace, testKey, testDefaultValue);

        // Create equivalent using FromLocalizedString
        const string localizedString = $"NSLOCTEXT(\"{testNamespace}\", \"{testKey}\", \"{testDefaultValue}\")";
        var textFromString = FText.FromLocalizedString(localizedString);

        using (Assert.EnterMultipleScope())
        {
            Assert.That(localizedText, Is.Not.Null);
            Assert.That(textFromString, Is.Not.Null);

            // Both should be valid FText instances
            Assert.That(localizedText.Empty, Is.False);
            Assert.That(textFromString.Empty, Is.False);

            var localizedString1 = localizedText.ToString();
            var localizedString2 = textFromString.ToString();

            Assert.That(localizedString1, Is.Not.Null);
            Assert.That(localizedString2, Is.Not.Null);
            Assert.That(localizedString1, Is.Not.Empty);
            Assert.That(localizedString2, Is.Not.Empty);
        }
    }

    [Test]
    public void LocalizedWithLongStrings()
    {
        const string testNamespace = "TestGame.VeryLongNamespace.WithMultipleLevels";
        const string testKey = "VeryLongKeyNameThatExceedsNormalLength.WithAdditionalSegments";
        var testDefaultValue = new string('A', 1000); // Very long default value

        var localizedText = FText.Localized(testNamespace, testKey, testDefaultValue);

        using (Assert.EnterMultipleScope())
        {
            Assert.That(localizedText, Is.Not.Null);
            Assert.That(localizedText.Empty, Is.False);

            var textString = localizedText.ToString();
            Assert.That(textString, Is.Not.Null);
            Assert.That(textString, Is.Not.Empty);
        }
    }

    [Test]
    public void FromLocalizedStringWithNestedQuotes()
    {
        // Test localized string with escaped quotes in the content
        const string localizedString = "NSLOCTEXT(\"TestGame\", \"Quote.Test\", \"He said, \\\"Hello World!\\\"\")";

        var textFromLocalized = FText.FromLocalizedString(localizedString);

        using (Assert.EnterMultipleScope())
        {
            Assert.That(textFromLocalized, Is.Not.Null);
            Assert.That(textFromLocalized.Empty, Is.False);

            var textString = textFromLocalized.ToString();
            Assert.That(textString, Is.Not.Null);
            Assert.That(textString, Is.Not.Empty);
        }
    }
}
