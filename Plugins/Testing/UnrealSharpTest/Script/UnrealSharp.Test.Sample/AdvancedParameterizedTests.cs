using NUnit.Framework;
using UnrealSharp.Test.Attributes;

namespace UnrealSharp.Test.Sample;

public class AdvancedParameterizedTests
{
    private static readonly int[] Numbers = [1, 2, 3, 4, 5];
    private static readonly string[] Names = ["Alice", "Bob", "Charlie"];

    [Combinatorial]
    public void Parameter_Level_Attributes_Values_And_Range([Values(1, 2)] [Range(5, 7)] int value)
    {
        // Will run with values: 1, 2, 5, 6, 7
        Assert.That(value, Is.AnyOf(1, 2, 5, 6, 7));
    }

    [Combinatorial]
    public void Parameter_Level_Attributes_With_Random([Values(10, 20)] [DynamicRandom(1, 5, 2)] int value)
    {
        // Will run with values: 10, 20, and 2 random numbers between 1 and 5
        Assert.That(value, Is.EqualTo(10).Or.EqualTo(20).Or.GreaterThanOrEqualTo(1).And.LessThanOrEqualTo(5));
    }

    [Combinatorial]
    public void Multiple_Parameters_With_Combined_Attributes(
        [Values("A", "B")] [ValueSource(nameof(Names))] string text,
        [Values(1, 2)] [Range(5, 6)] int number
    )
    {
        // text will have values: "A", "B", "Alice", "Bob", "Charlie"
        // number will have values: 1, 2, 5, 6
        using (Assert.EnterMultipleScope())
        {
            Assert.That(text, Is.Not.Empty);
            Assert.That(number, Is.AnyOf(1, 2, 5, 6));
        }
    }

    [Test]
    public void ValueSource_And_Values_Combination([ValueSource(nameof(Numbers))] [Values(10, 20)] int value)
    {
        // Will run with values: 1, 2, 3, 4, 5, 10, 20
        Assert.That(value, Is.GreaterThan(0));
    }

    private static readonly (int min, int max)[] RangePairs = [(1, 3), (4, 6)];

    [Combinatorial]
    public void Dynamic_Range_With_Values(
        [ValueSource(nameof(RangePairs))] (int min, int max) range,
        [Values(0)] [Range(7, 8)] int value
    )
    {
        using (Assert.EnterMultipleScope())
        {
            // range will be (1,3) and (4,6)
            // value will be 0, 7, 8
            Assert.That(value, Is.AnyOf(0, 7, 8));
            Assert.That(range.max, Is.GreaterThan(range.min));
        }
    }

    [Test]
    public void Values_With_Custom_Names([Values("Test1", "Test2")] string value)
    {
        Assert.That(value, Is.AnyOf("Test1", "Test2"));
    }

    [Pairwise]
    public void Pairwise_With_Multiple_Attributes(
        [Values("X", "Y", "Z")] string letter,
        [Values(true, false)] bool flag
    )
    {
        // Will generate combinations of:
        // letter: "X", "Y", "Z"
        // flag: true, false
        Assert.That(letter, Is.AnyOf("X", "Y", "Z"));
    }

    [Sequential]
    public void Sequential_With_Multiple_Value_Sources(
        [Values(1, 2)] [Range(5, 6)] int first,
        [Values("A", "B", "C")] string second
    )
    {
        // Will sequence through:
        // first: 1, 2, 5, 6
        // second: "A", "B", "C"
        using (Assert.EnterMultipleScope())
        {
            Assert.That(first, Is.AnyOf(1, 2, 5, 6));
            Assert.That(second, Is.AnyOf("A", "B", "C"));
        }
    }
}
