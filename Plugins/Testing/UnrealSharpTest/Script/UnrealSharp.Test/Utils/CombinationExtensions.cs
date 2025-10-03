namespace UnrealSharp.Test.Utils;

/// <summary>
/// Provides extension methods for generating and manipulating combinations
/// of sequences, including Cartesian product, sequential grouping, and pairwise grouping.
/// </summary>
public static class CombinationExtensions
{
    /// <summary>
    /// Generates the Cartesian product of sequences, resulting in all possible combinations
    /// by taking one element from each sequence.
    /// </summary>
    /// <param name="sequences">A collection of collections from which the Cartesian product is to be generated.</param>
    /// <typeparam name="T">The type of elements in the input sequences.</typeparam>
    /// <returns>
    /// A collection of combinations, where each combination is represented as a collection
    /// containing one element from each of the input sequences.
    /// </returns>
    public static IEnumerable<IEnumerable<T>> CartesianProduct<T>(this IEnumerable<IEnumerable<T>> sequences)
    {
        return sequences.Aggregate(
            (IEnumerable<IEnumerable<T>>)
                [
                    [],
                ],
            (acc, src) => src.SelectMany(x => acc.Select(a => a.Concat([x])))
        );
    }

    /// <summary>
    /// Groups sequences into sequentially aligned combinations,
    /// where elements from each sequence are taken in order, producing a collection
    /// containing one element from each sequence at each step.
    /// </summary>
    /// <param name="sequences">A collection of sequences to group sequentially.</param>
    /// <typeparam name="T">The type of elements in the sequences.</typeparam>
    /// <returns>
    /// A collection of combinations, where each combination is represented as an array
    /// containing elements from the same index position of the input sequences.
    /// If a sequence is shorter, default values will be used to fill the missing elements.
    /// </returns>
    public static IEnumerable<IEnumerable<T>> SequentialGrouping<T>(this IEnumerable<IEnumerable<T>> sequences)
    {
        var enumerators = sequences.Select(x => x.GetEnumerator()).ToList();
        var currentValues = new T[enumerators.Count];
        var hasValue = new bool[enumerators.Count];

        try
        {
            while (true)
            {
                var anyMoved = false;

                for (var i = 0; i < enumerators.Count; i++)
                {
                    if (enumerators[i].MoveNext())
                    {
                        currentValues[i] = enumerators[i].Current;
                        hasValue[i] = true;
                        anyMoved = true;
                    }
                    else if (!hasValue[i])
                    {
                        currentValues[i] = default!;
                    }
                }

                if (!anyMoved)
                    break;

                yield return currentValues.ToArray();
            }
        }
        finally
        {
            foreach (var enumerator in enumerators)
            {
                enumerator.Dispose();
            }
        }
    }

    /// <summary>
    /// Generates test case combinations using pairwise testing by covering all possible pairs of parameter values across input sequences.
    /// </summary>
    /// <param name="sequences">A collection of collections where each collection represents a set of possible parameter values.</param>
    /// <typeparam name="T">The type of elements in the input sequences.</typeparam>
    /// <returns>
    /// A collection of combinations, where each combination is represented as a collection containing one value from each input sequence,
    /// ensuring all possible pairs of values are covered across different sequences.
    /// </returns>
    public static IEnumerable<IEnumerable<T>> PairwiseGrouping<T>(this IEnumerable<IEnumerable<T>> sequences)
    {
        var parameterValues = sequences.Select(seq => seq.ToArray()).ToArray();
        switch (parameterValues.Length)
        {
            case 0:
                return [];
            case 1:
                return parameterValues[0].Select(v => new[] { v });
        }

        var parameterSizes = parameterValues.Select(p => p.Length).ToArray();

        var testCases = new List<int[]>();

        var coveredPairs = new HashSet<(int, int, int, int)>();

        var firstCase = new int[parameterSizes.Length];
        testCases.Add(firstCase);

        var totalPairsNeeded = 0;
        for (var i = 0; i < parameterSizes.Length - 1; i++)
        {
            for (var j = i + 1; j < parameterSizes.Length; j++)
            {
                totalPairsNeeded += parameterSizes[i] * parameterSizes[j];
            }
        }

        while (coveredPairs.Count < totalPairsNeeded)
        {
            var bestCase = GenerateBestTestCase(parameterSizes, coveredPairs);
            if (bestCase == null)
                break;

            testCases.Add(bestCase);

            // Add all pairs from this test case to covered pairs
            for (var i = 0; i < bestCase.Length - 1; i++)
            {
                for (var j = i + 1; j < bestCase.Length; j++)
                {
                    coveredPairs.Add((i, bestCase[i], j, bestCase[j]));
                }
            }
        }

        return testCases.Select(testCase => testCase.Select((value, paramIndex) => parameterValues[paramIndex][value]));
    }

    private static int[]? GenerateBestTestCase(int[] parameterSizes, HashSet<(int, int, int, int)> coveredPairs)
    {
        var bestCase = new int[parameterSizes.Length];
        var maxNewPairs = -1;

        // Try different combinations to find the one that covers the most new pairs
        for (var attempts = 0; attempts < 50; attempts++)
        {
            var candidate = new int[parameterSizes.Length];
            for (var i = 0; i < candidate.Length; i++)
            {
                candidate[i] = Random.Shared.Next(parameterSizes[i]);
            }

            var newPairs = CountNewPairs(candidate, coveredPairs);
            if (newPairs <= maxNewPairs)
                continue;
            maxNewPairs = newPairs;
            Array.Copy(candidate, bestCase, candidate.Length);
        }

        return maxNewPairs > 0 ? bestCase : null;
    }

    private static int CountNewPairs(int[] candidate, HashSet<(int, int, int, int)> coveredPairs)
    {
        var newPairs = 0;
        for (var i = 0; i < candidate.Length - 1; i++)
        {
            for (var j = i + 1; j < candidate.Length; j++)
            {
                if (!coveredPairs.Contains((i, candidate[i], j, candidate[j])))
                {
                    newPairs++;
                }
            }
        }
        return newPairs;
    }
}
