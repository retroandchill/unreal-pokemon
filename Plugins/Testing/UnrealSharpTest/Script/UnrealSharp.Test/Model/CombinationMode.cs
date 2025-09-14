namespace UnrealSharp.Test.Model;

/// <summary>
/// Represents the mode in which parameter combinations are generated for testing.
/// </summary>
/// <remarks>
/// This enumeration is used to specify how combinations of parameters for test methods
/// should be generated when tests are executed. The choice of combination mode impacts
/// the efficiency, coverage, and order of test execution.
/// </remarks>
public enum CombinationMode
{
    /// <summary>
    /// Test cases are built using a cartesian product of parameter combinations.
    /// </summary>
    Combinatorial,

    /// <summary>
    /// Test cases are built using a sequence of parameter combinations.
    /// </summary>
    Sequential,

    /// <summary>
    /// Test cases are built using where all possible combinations of parameters are tested.
    /// </summary>
    Pairwise,
}
