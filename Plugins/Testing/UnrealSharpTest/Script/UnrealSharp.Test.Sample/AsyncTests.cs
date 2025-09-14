using NUnit.Framework;

namespace UnrealSharp.Test.Sample;

public class AsyncTests
{
    // Basic async Task test cases
    [TestCase(100, 200, 300)]
    [TestCase(0, 0, 0)]
    [TestCase(-50, -30, -80)]
    public async Task AddAsync_SimpleValues_ReturnsExpectedSum(int a, int b, int expected)
    {
        var result = await CalculateAsync(a, b);
        Assert.That(result, Is.EqualTo(expected));
    }

    // ValueTask test cases with TestCaseSource
    private static readonly TestCaseData[] MultiplicationTestCases =
    [
        new TestCaseData(5, 2).Returns(10),
        new TestCaseData(0, 100).Returns(0),
        new TestCaseData(-3, 4).Returns(-12),
    ];

    [TestCaseSource(nameof(MultiplicationTestCases))]
    public async ValueTask<int> MultiplyAsync_ValidValues_ReturnsExpectedResult(int x, int y)
    {
        return await CalculateProductAsync(x, y);
    }

    // Task<T> with complex return type
    private static IEnumerable<object[]> ComplexTestCases()
    {
        yield return [new Point(1, 1), new Point(2, 2), new Point(3, 3)];
        yield return [new Point(0, 5), new Point(0, 3), new Point(0, 8)];
        yield return [new Point(-1, 2), new Point(1, -2), new Point(0, 0)];
    }

    [TestCaseSource(nameof(ComplexTestCases))]
    public async Task<Point> AddPointsAsync_ValidPoints_ReturnsExpectedPoint(
        Point a,
        Point b,
        Point expected
    )
    {
        var result = await AddPointsAsync(a, b);
        using (Assert.EnterMultipleScope())
        {
            Assert.That(result.X, Is.EqualTo(expected.X));
            Assert.That(result.Y, Is.EqualTo(expected.Y));
        }
        return result;
    }

    // ValueTask<T> with nullable complex type
    private static IEnumerable<object?[]> NullableTestCases()
    {
        yield return [new ComplexValue("Test1", 10), true];
        yield return [null, false];
    }

    [TestCaseSource(nameof(NullableTestCases))]
    public async ValueTask ProcessComplexAsync_HandlesNullables(
        ComplexValue? value,
        bool shouldSucceed
    )
    {
        if (shouldSucceed)
        {
            var result = await ProcessComplexValueAsync(value!);
            Assert.That(result, Is.True);
        }
        else
        {
            Assert.ThrowsAsync<ArgumentNullException>(async () =>
                await ProcessComplexValueAsync(value!)
            );
        }
    }

    // Helper methods
    private static async Task<int> CalculateAsync(int a, int b)
    {
        await Task.Delay(10); // Simulate async work
        return a + b;
    }

    private static async ValueTask<int> CalculateProductAsync(int x, int y)
    {
        await Task.Delay(10); // Simulate async work
        return x * y;
    }

    private static async Task<Point> AddPointsAsync(Point a, Point b)
    {
        await Task.Delay(10); // Simulate async work
        return new Point(a.X + b.X, a.Y + b.Y);
    }

    private static async ValueTask<bool> ProcessComplexValueAsync(ComplexValue value)
    {
        ArgumentNullException.ThrowIfNull(value);
        await Task.Delay(10); // Simulate async work
        return value.Value > 0;
    }

    // Test with cancellation token
    [TestCase(100)]
    [TestCase(500)]
    public Task OperationWithCancellation_CancelsAsExpected(int delayMs)
    {
        using var cts = new CancellationTokenSource(delayMs / 2);

        Assert.ThrowsAsync<TaskCanceledException>(async () => await Operation());
        return Task.CompletedTask;

        async Task Operation()
        {
            await Task.Delay(delayMs, cts.Token);
        }
    }
}
