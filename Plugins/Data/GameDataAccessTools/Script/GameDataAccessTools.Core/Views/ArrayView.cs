using Retro.ReadOnlyParams.Annotations;
using UnrealSharp;
using UnrealSharp.Core;
using ZLinq;

namespace GameDataAccessTools.Core.Views;

/// <summary>
/// Represents a read-only view over an unmanaged array, providing a managed interface for iteration and element access.
/// </summary>
/// <typeparam name="T">
/// The type of elements in the array, constrained to types that satisfy the `ref struct` restriction.
/// </typeparam>
/// <param name="nativeArray">The native array to be wrapped by this view.</param>
/// <param name="fromNative">Delegate to convert a native pointer and index to a managed element.</param>
public readonly ref struct ArrayView<T>(ref UnmanagedArray nativeArray, [ReadOnly] Func<IntPtr, int, T> fromNative)
    where T : allows ref struct
{
    private readonly ref UnmanagedArray _nativeArray = ref nativeArray;

    /// <summary>
    /// Gets the number of elements in the array represented by this view.
    /// </summary>
    /// <value>
    /// The total number of elements in the underlying array.
    /// If the array is empty, the value will be 0.
    /// </value>
    public int Length => _nativeArray.ArrayNum;

    /// <summary>
    /// Gets a value indicating whether the array represented by this view is empty.
    /// </summary>
    /// <value>
    /// True if the array contains no elements; otherwise, false.
    /// </value>
    public bool IsEmpty => _nativeArray.ArrayNum == 0;

    /// <summary>
    /// Provides access to an element in the unmanaged array at the specified index.
    /// </summary>
    /// <param name="index">
    /// The zero-based index of the element to access in the array.
    /// </param>
    /// <returns>
    /// The managed representation of the array element at the specified index.
    /// </returns>
    /// <exception cref="ArgumentOutOfRangeException">
    /// Thrown when the specified index is less than zero or greater than or equal to the array length.
    /// </exception>
    public T this[int index]
    {
        get
        {
            if (index < 0 || index >= _nativeArray.ArrayNum)
            {
                throw new ArgumentOutOfRangeException(nameof(index));
            }

            return fromNative(_nativeArray.Data, index);
        }
    }

    /// <summary>
    /// Returns an enumerator that iterates through the elements of the <see cref="ArrayView{T}"/>.
    /// The enumerator provides a managed interface to traverse the underlying unmanaged array.
    /// </summary>
    /// <returns>
    /// An <see cref="ArrayView{T}.Enumerator"/> that can be used to iterate over the elements of the array view.
    /// </returns>
    public Enumerator GetEnumerator()
    {
        return new Enumerator(this);
    }

    /// <summary>
    /// Provides a managed enumerator for iterating over the elements of an <see cref="ArrayView{T}"/>.
    /// The enumerator enables traversal of the underlying unmanaged array in a controlled and sequential manner.
    /// </summary>
    /// <typeparam name="T">
    /// The type of elements in the array, constrained to types that satisfy the `ref struct` restriction.
    /// </typeparam>
    public ref struct Enumerator
    {
        internal Enumerator(ArrayView<T> arrayView)
        {
            _arrayView = arrayView;
        }

        private readonly ArrayView<T> _arrayView;
        private int _index = -1;

        /// <summary>
        /// Gets the number of elements in the array represented by the current <see cref="ArrayView{T}"/> instance.
        /// </summary>
        /// <value>
        /// The total number of elements in the underlying unmanaged array.
        /// Returns 0 if the array is empty.
        /// </value>
        public int Length => _arrayView.Length;

        /// <summary>
        /// Gets the current element in the sequence represented by the enumerator.
        /// </summary>
        /// <value>
        /// The element in the array at the current position of the enumerator.
        /// If the enumerator is positioned before the first element or after the last element, this property is undefined.
        /// </value>
        public T Current => _arrayView[_index];

        /// <summary>
        /// Advances the enumerator to the next element of the <see cref="ArrayView{T}"/>.
        /// </summary>
        /// <returns>
        /// <c>true</c> if the enumerator was successfully advanced to the next element;
        /// <c>false</c> if the enumerator has passed the end of the array view.
        /// </returns>
        public bool MoveNext()
        {
            _index++;
            return _index < _arrayView.Length;
        }
    }
}

/// <summary>
/// Serves as a value enumerator for iterating over elements in an <see cref="ArrayView{T}"/> instance.
/// </summary>
/// <typeparam name="T">
/// The type of elements in the collection being enumerated.
/// </typeparam>
/// <remarks>
/// This structure provides efficient enumeration over an <see cref="ArrayView{T}"/> without allocating heap memory.
/// Iteration is encapsulated within the structure, and the enumerator is disposed of after use to manage resources.
/// </remarks>
public ref struct ArrayValueEnumerator<T> : IValueEnumerator<T>
{
    private ArrayView<T>.Enumerator _enumerator;

    internal ArrayValueEnumerator(ArrayView<T> arrayView)
    {
        _enumerator = arrayView.GetEnumerator();
    }

    /// <inheritdoc />
    public bool TryGetNext(out T current)
    {
        if (_enumerator.MoveNext())
        {
            current = _enumerator.Current;
            return true;
        }

        current = default!;
        return false;
    }

    /// <inheritdoc />
    public bool TryGetNonEnumeratedCount(out int count)
    {
        count = _enumerator.Length;
        return true;
    }

    /// <inheritdoc />
    public bool TryGetSpan(out ReadOnlySpan<T> span)
    {
        span = ReadOnlySpan<T>.Empty;
        return false;
    }

    /// <inheritdoc />
    public bool TryCopyTo(scoped Span<T> destination, Index offset) => false;

    /// <inheritdoc />
    public void Dispose()
    {
        // TODO release managed resources here
    }
}

/// <summary>
/// Provides an enumerator for iterating over the values of an array view, where each element
/// of the array is represented by a struct and transformed into a value of a specified type
/// via a selector function.
/// </summary>
/// <typeparam name="TStruct">
/// The type of the struct stored in the array. Must be a `struct` that implements the <see cref="MarshalledStruct{Self}"/> interface
/// and satisfies the `ref struct` constraint.
/// </typeparam>
/// <typeparam name="TValue">
/// The type of the value produced by the selector function for each element.
/// </typeparam>
public ref struct StructArrayValueEnumerator<TStruct, TValue> : IValueEnumerator<TValue>
    where TStruct : struct, MarshalledStruct<TStruct>, allows ref struct
{
    private ArrayView<StructView<TStruct>>.Enumerator _enumerator;
    private readonly Func<StructView<TStruct>, TValue> _selector;

    internal StructArrayValueEnumerator(
        ArrayView<StructView<TStruct>> arrayView,
        Func<StructView<TStruct>, TValue> selector
    )
    {
        _enumerator = arrayView.GetEnumerator();
        _selector = selector;
    }

    /// <inheritdoc />
    public bool TryGetNext(out TValue current)
    {
        if (_enumerator.MoveNext())
        {
            current = _selector(_enumerator.Current);
            return true;
        }

        current = default!;
        return false;
    }

    /// <inheritdoc />
    public bool TryGetNonEnumeratedCount(out int count)
    {
        count = _enumerator.Length;
        return true;
    }

    /// <inheritdoc />
    public bool TryGetSpan(out ReadOnlySpan<TValue> span)
    {
        span = ReadOnlySpan<TValue>.Empty;
        return false;
    }

    /// <inheritdoc />
    public bool TryCopyTo(scoped Span<TValue> destination, Index offset) => false;

    /// <inheritdoc />
    public void Dispose()
    {
        // Nothing to dispose of
    }
}

/// <summary>
/// Provides extension methods for working with <see cref="ArrayView{T}"/> types, enabling additional functionality
/// such as transforming these array views into enumerable representations.
/// </summary>
public static class ArrayViewExtensions
{
    /// <summary>
    /// Converts the current <see cref="ArrayView{T}"/> into a value enumerable representation,
    /// allowing it to be enumerated efficiently without allocations.
    /// </summary>
    /// <typeparam name="T">The type of elements in the <see cref="ArrayView{T}"/>.</typeparam>
    /// <param name="arrayView">The array view to convert into a value enumerable.</param>
    /// <returns>
    /// A <see cref="ValueEnumerable{TEnumerator, T}"/> representing the enumerable version of the
    /// provided array view, using an <see cref="ArrayValueEnumerator{T}"/> to iterate through the elements.
    /// </returns>
    public static ValueEnumerable<ArrayValueEnumerator<T>, T> AsValueEnumerable<T>(this ArrayView<T> arrayView)
    {
        return new ValueEnumerable<ArrayValueEnumerator<T>, T>(new ArrayValueEnumerator<T>(arrayView));
    }

    /// <summary>
    /// Converts the <see cref="ArrayView{TValue}"/> to a <see cref="ValueEnumerable{TEnumerator, TStruct}"/>.
    /// This allows enumeration of the array view as a strongly-typed value enumerable.
    /// </summary>
    /// <typeparam name="TStruct">
    /// The type of the struct contained in the <see cref="StructView{TStruct}"/> elements of the array view.
    /// Must be a value type that implements <see cref="MarshalledStruct{TStruct}"/>.
    /// </typeparam>
    /// <param name="arrayView">
    /// The <see cref="ArrayView{TValue}"/> to be converted.
    /// </param>
    /// <returns>
    /// A <see cref="ValueEnumerable{TEnumerator, TStruct}"/>
    /// representing the elements of the array view for value-based enumeration.
    /// </returns>
    public static ValueEnumerable<StructArrayValueEnumerator<TStruct, TStruct>, TStruct> AsValueEnumerable<TStruct>(
        this ArrayView<StructView<TStruct>> arrayView
    )
        where TStruct : struct, MarshalledStruct<TStruct>
    {
        return new ValueEnumerable<StructArrayValueEnumerator<TStruct, TStruct>, TStruct>(
            new StructArrayValueEnumerator<TStruct, TStruct>(arrayView, v => v.Copy())
        );
    }

    /// <summary>
    /// Converts the elements of the given <see cref="ArrayView{T}"/> to a <see cref="ValueEnumerable{TEnumerator, TValue}"/> sequence.
    /// This allows iteration over the transformed elements using a custom selector function for each element.
    /// </summary>
    /// <typeparam name="TStruct">The type of the struct within the <see cref="StructView{TStruct}"/>.</typeparam>
    /// <typeparam name="TValue">The type of values resulting from the provided selector function.</typeparam>
    /// <param name="arrayView">The <see cref="ArrayView{T}"/> containing the elements to enumerate.</param>
    /// <param name="selector">A transformation function applied to each element of the <see cref="ArrayView{T}"/>.</param>
    /// <returns>
    /// A <see cref="ValueEnumerable{TEnumerator, TValue}"/> that can be used to iterate over the transformed elements.
    /// </returns>
    public static ValueEnumerable<StructArrayValueEnumerator<TStruct, TValue>, TValue> AsValueEnumerable<
        TStruct,
        TValue
    >(this ArrayView<StructView<TStruct>> arrayView, Func<StructView<TStruct>, TValue> selector)
        where TStruct : struct, MarshalledStruct<TStruct>, allows ref struct
    {
        return new ValueEnumerable<StructArrayValueEnumerator<TStruct, TValue>, TValue>(
            new StructArrayValueEnumerator<TStruct, TValue>(arrayView, selector)
        );
    }
}
