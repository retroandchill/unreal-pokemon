using Retro.ReadOnlyParams.Annotations;
using UnrealSharp;
using UnrealSharp.Core;
using ZLinq;

namespace GameDataAccessTools.Core.Views;

public readonly ref struct ArrayView<T>(
    ref UnmanagedArray nativeArray,
    [ReadOnly] Func<IntPtr, int, T> fromNative
)
    where T : allows ref struct
{
    private readonly ref UnmanagedArray _nativeArray = ref nativeArray;

    public int Length => _nativeArray.ArrayNum;

    public bool IsEmpty => _nativeArray.ArrayNum == 0;

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

    public Enumerator GetEnumerator()
    {
        return new Enumerator(this);
    }

    public ref struct Enumerator
    {
        internal Enumerator(ArrayView<T> arrayView)
        {
            _arrayView = arrayView;
        }

        private readonly ArrayView<T> _arrayView;
        private int _index = -1;

        public int Length => _arrayView.Length;

        public T Current => _arrayView[_index];

        public bool MoveNext()
        {
            _index++;
            return _index < _arrayView.Length;
        }
    }
}

public ref struct ArrayValueEnumerator<T> : IValueEnumerator<T>
{
    private ArrayView<T>.Enumerator _enumerator;

    internal ArrayValueEnumerator(ArrayView<T> arrayView)
    {
        _enumerator = arrayView.GetEnumerator();
    }

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

    public bool TryGetNonEnumeratedCount(out int count)
    {
        count = _enumerator.Length;
        return true;
    }

    public bool TryGetSpan(out ReadOnlySpan<T> span)
    {
        span = ReadOnlySpan<T>.Empty;
        return false;
    }

    public bool TryCopyTo(scoped Span<T> destination, Index offset) => false;

    public void Dispose()
    {
        // TODO release managed resources here
    }
}

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

    public bool TryGetNonEnumeratedCount(out int count)
    {
        count = _enumerator.Length;
        return true;
    }

    public bool TryGetSpan(out ReadOnlySpan<TValue> span)
    {
        span = ReadOnlySpan<TValue>.Empty;
        return false;
    }

    public bool TryCopyTo(scoped Span<TValue> destination, Index offset) => false;

    public void Dispose()
    {
        // Nothing to dispose of
    }
}

public static class ArrayViewExtensions
{
    public static ValueEnumerable<ArrayValueEnumerator<T>, T> AsValueEnumerable<T>(
        this ArrayView<T> arrayView
    )
    {
        return new ValueEnumerable<ArrayValueEnumerator<T>, T>(
            new ArrayValueEnumerator<T>(arrayView)
        );
    }

    public static ValueEnumerable<
        StructArrayValueEnumerator<TStruct, TStruct>,
        TStruct
    > AsValueEnumerable<TStruct>(this ArrayView<StructView<TStruct>> arrayView)
        where TStruct : struct, MarshalledStruct<TStruct>
    {
        return new ValueEnumerable<StructArrayValueEnumerator<TStruct, TStruct>, TStruct>(
            new StructArrayValueEnumerator<TStruct, TStruct>(arrayView, v => v.Copy())
        );
    }

    public static ValueEnumerable<
        StructArrayValueEnumerator<TStruct, TValue>,
        TValue
    > AsValueEnumerable<TStruct, TValue>(
        this ArrayView<StructView<TStruct>> arrayView,
        Func<StructView<TStruct>, TValue> selector
    )
        where TStruct : struct, MarshalledStruct<TStruct>, allows ref struct
    {
        return new ValueEnumerable<StructArrayValueEnumerator<TStruct, TValue>, TValue>(
            new StructArrayValueEnumerator<TStruct, TValue>(arrayView, selector)
        );
    }
}
