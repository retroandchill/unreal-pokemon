namespace UnrealSharp.Test.Model;

/// <summary>
/// Flags for indicating the matching type to use for an expected message.
/// </summary>
public enum EAutomationExpectedMessageFlags
{
    /// <summary>
    /// When matching expected messages, do so exactly.
    /// </summary>
    Exact,

    /// <summary>
    /// When matching expected messages, just see if the message string is contained in the string to be evaluated.
    /// </summary>
    Contains,
}
