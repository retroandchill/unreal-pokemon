using NUnit.Framework;

namespace UnrealSharp.Test.Sample;

public record Point(int X, int Y);

// Class with deconstruct method
public class ComplexValue(string name, int value)
{
    public string Name { get; } = name;
    public int Value { get; } = value;

    public void Deconstruct(out string name, out int value)
    {
        name = Name;
        value = Value;
    }
}

public class ParameterizedTests
{
    // Basic parameterized test using TestCase attribute
    [TestCase(1, 2, 3)]
    [TestCase(0, 0, 0)]
    [TestCase(-1, -2, -3)]
    public void Add_SimpleValues_ReturnsExpectedSum(int a, int b, int expected)
    {
        var result = a + b;
        Assert.That(result, Is.EqualTo(expected));
    }

    // Test with multiple parameters of different types
    [TestCase("hello", 3, "hellohellohello")]
    [TestCase("", 5, "")]
    [TestCase("a", 0, "")]
    public void Repeat_String_ReturnsExpectedResult(string input, int times, string expected)
    {
        var result = string.Concat(Enumerable.Repeat(input, times));
        Assert.That(result, Is.EqualTo(expected));
    }

    // Using TestCaseSource for more complex test cases
    private static readonly TestCaseData[] DivisionTestCases =
    [
        new TestCaseData(10, 2).Returns(5),
        new TestCaseData(0, 5).Returns(0),
        new TestCaseData(-15, 3).Returns(-5),
        new TestCaseData(1, 3).Returns(0),
    ];

    [TestCaseSource(nameof(DivisionTestCases))]
    public int Divide_ValidValues_ReturnsExpectedResult(int n, int d)
    {
        return n / d;
    }

    // Using object?[] for test cases
    private static IEnumerable<object?[]> ArrayTestCases()
    {
        yield return [10, 20];
        yield return [-5, 0];
        yield return [null, 42];
        yield return [100, null];
    }

    [TestCaseSource(nameof(ArrayTestCases))]
    public void Process_ObjectArray_HandlesNullables(int? a, int? b)
    {
        if (a.HasValue && b.HasValue)
        {
            Assert.That(a.Value + b.Value, Is.GreaterThanOrEqualTo(a.Value));
        }
        else
        {
            Assert.That(a ?? b, Is.Not.Null);
        }
    }

    // Mixing both approaches
    private static IEnumerable<object?[]> MixedTestCases()
    {
        yield return [new Point(1, 1), "Scale1"];
        yield return [new Point(0, 0), "Origin"];
        yield return [new Point(-1, -1), null];
    }

    [TestCaseSource(nameof(MixedTestCases))]
    public void Process_MixedInputs_HandlesAllCases(Point point, string? label)
    {
        var (x, y) = point; // Using deconstruct

        if (label == null)
        {
            Assert.That(x, Is.EqualTo(y), "Coordinates should be equal when label is null");
        }
        else
        {
            Assert.That(label, Is.Not.Empty);
        }
    }

    // Using nullable complex types
    private static IEnumerable<object?[]> NullableComplexTestCases()
    {
        return
        [
            [new ComplexValue("Valid", 1), true],
            [null, false],
        ];
    }

    [TestCaseSource(nameof(NullableComplexTestCases))]
    public void Process_NullableComplex_HandlesNullCases(ComplexValue? value, bool expectValid)
    {
        if (expectValid)
        {
            Assert.That(value, Is.Not.Null);
            var (name, val) = value!; // Using deconstruct after null check
            Assert.That(name, Is.Not.Empty);
            Assert.That(val, Is.GreaterThan(0));
        }
        else
        {
            Assert.That(value, Is.Null);
        }
    }

    // Using ValueSource for reusable test data
    private static readonly string[] Names = ["Alice", "Bob", "Charlie"];

    [Test]
    public void Values_FromValueSource_AreNotEmpty([ValueSource(nameof(Names))] string name)
    {
        Assert.That(name, Is.Not.Empty);
        Assert.That(name.Length, Is.GreaterThan(0));
    }

    // Combinatorial test cases
    [TestCase("valid", 100)]
    [TestCase("", 0)]
    [TestCase(null, -1)]
    public void Process_DifferentInputs_HandlesEdgeCases(string? input, int value)
    {
        switch (input)
        {
            case null:
                Assert.Throws<ArgumentNullException>(() => ProcessInput(input, value));
                break;
            case "":
                Assert.Throws<ArgumentException>(() => ProcessInput(input, value));
                break;
            default:
            {
                var result = ProcessInput(input, value);
                Assert.That(result, Is.Not.Null);
                Assert.That(result, Does.Contain(input));
                Assert.That(result, Does.Contain(value.ToString()));
                break;
            }
        }
    }

    // Helper method for the above test
    private static string ProcessInput(string? input, int value)
    {
        ArgumentNullException.ThrowIfNull(input);
        return string.IsNullOrEmpty(input)
            ? throw new ArgumentException("Input cannot be empty", nameof(input))
            : $"{input}:{value}";
    }

    // Range values test using TestCase
    [TestCase(1)]
    [TestCase(50)]
    [TestCase(100)]
    public void Values_InRange_AreValid(int value)
    {
        Assert.That(value, Is.InRange(1, 100));
    }

    // Parameterized test with custom test name
    [TestCase(true, TestName = "IsValid returns true for valid input")]
    [TestCase(false, TestName = "IsValid returns false for invalid input")]
    public void IsValid_Returns_ExpectedResult(bool expected)
    {
        var result = expected; // Simplified for example
        Assert.That(result, Is.EqualTo(expected));
    }
}
