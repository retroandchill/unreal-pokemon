using NUnit.Framework;

namespace UnrealSharp.Test.Sample;

public class SyncTaskTests
{
    [Test]
    public async Task CompletedTask_ReturnsImmediately()
    {
        // Task that's already completed when returned
        var result = await GetCompletedTask();
        Assert.That(result, Is.EqualTo(42));
    }

    [Test]
    public async Task CompletedValueTask_ReturnsImmediately()
    {
        // ValueTask that's already completed when returned
        var result = await GetCompletedValueTask();
        Assert.That(result, Is.EqualTo(42));
    }

    [TestCase(1)]
    [TestCase(2)]
    [TestCase(3)]
    public async Task CachedResult_ReturnsCompletedTask(int value)
    {
        var cache = new Dictionary<int, Task<int>> { [value] = Task.FromResult(value * 2) };

        var result = await GetCachedValueAsync(value, cache);
        Assert.That(result, Is.EqualTo(value * 2));
    }

    [TestCase("test")]
    [TestCase("sample")]
    public async Task CompletedValueTask_WithParameter_ReturnsImmediately(string input)
    {
        var result = await ProcessImmediatelyAsync(input);
        Assert.That(result, Is.EqualTo(input.ToUpperInvariant()));
    }

    [Test]
    public async Task FromResult_TaskCompletion_Status()
    {
        var task = GetCompletedTask();
        using (Assert.EnterMultipleScope())
        {
            // Verify the task is already completed
            Assert.That(task.IsCompleted, Is.True);
            Assert.That(task.Status, Is.EqualTo(TaskStatus.RanToCompletion));
        }

        var result = await task;
        Assert.That(result, Is.EqualTo(42));
    }

    [Test]
    public async Task FromResult_ValueTaskCompletion_Status()
    {
        var valueTask = GetCompletedValueTask();

        // Verify the ValueTask is already completed
        Assert.That(valueTask.IsCompleted, Is.True);

        var result = await valueTask;
        Assert.That(result, Is.EqualTo(42));
    }

    // Helper methods that return immediately completed tasks
    private static Task<int> GetCompletedTask()
    {
        return Task.FromResult(42);
    }

    private static ValueTask<int> GetCompletedValueTask()
    {
        return ValueTask.FromResult(42);
    }

    private static Task<int> GetCachedValueAsync(int key, Dictionary<int, Task<int>> cache)
    {
        return cache.TryGetValue(key, out var cachedValue) ? cachedValue : Task.FromResult(0);
    }

    private static ValueTask<string> ProcessImmediatelyAsync(string input)
    {
        // Synchronously complete and return result
        return ValueTask.FromResult(input.ToUpperInvariant());
    }

    [Test]
    public Task CompletedTask_WithException_ThrowsImmediately()
    {
        var exception = Assert.ThrowsAsync<InvalidOperationException>(async () =>
            await Task.FromException<int>(new InvalidOperationException("Immediate error"))
        );

        Assert.That(exception?.Message, Is.EqualTo("Immediate error"));
        return Task.CompletedTask;
    }

    [Test]
    public Task CompletedValueTask_WithException_ThrowsImmediately()
    {
        var exception = Assert.ThrowsAsync<InvalidOperationException>(async () =>
            await ValueTask.FromException<int>(new InvalidOperationException("Immediate error"))
        );

        Assert.That(exception?.Message, Is.EqualTo("Immediate error"));
        return Task.CompletedTask;
    }
}
