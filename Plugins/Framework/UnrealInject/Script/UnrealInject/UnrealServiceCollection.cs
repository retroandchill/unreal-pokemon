using System.Collections;
using Microsoft.Extensions.DependencyInjection;

namespace UnrealInject;

public class UnrealServiceCollection : IServiceCollection
{
    private readonly OrderedDictionary<Guid, ServiceDescriptor> _registrations = new();

    public int Count => _registrations.Count;

    public bool IsReadOnly => false;

    public ServiceDescriptor this[int index]
    {
        get => _registrations.GetAt(index).Value;
        set => _registrations.SetAt(index, value);
    }

    public ServiceDescriptor this[Guid key]
    {
        get => _registrations[key];
        set => _registrations[key] = value;
    }

    public IEnumerator<ServiceDescriptor> GetEnumerator()
    {
        return _registrations.Values.GetEnumerator();
    }

    IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();

    void ICollection<ServiceDescriptor>.Add(ServiceDescriptor item)
    {
        Add(item);
    }

    public Guid Add(ServiceDescriptor item)
    {
        var guid = Guid.CreateVersion7();
        _registrations.Add(guid, item);
        return guid;
    }

    public void Clear()
    {
        _registrations.Clear();
    }

    public bool Contains(ServiceDescriptor item)
    {
        return _registrations.ContainsValue(item);
    }

    public void CopyTo(ServiceDescriptor[] array, int arrayIndex)
    {
        _registrations.Values.CopyTo(array, arrayIndex);
    }

    public int IndexOf(ServiceDescriptor item)
    {
        for (var i = 0; i < _registrations.Count; i++)
        {
            if (_registrations.GetAt(i).Value != item)
                continue;

            return i;
        }

        return -1;
    }

    void IList<ServiceDescriptor>.Insert(int index, ServiceDescriptor item)
    {
        Insert(index, item);
    }

    public Guid Insert(int index, ServiceDescriptor item)
    {
        var guid = Guid.NewGuid();
        _registrations.Insert(index, guid, item);
        return guid;
    }

    public bool Remove(ServiceDescriptor item)
    {
        var itemIndex = IndexOf(item);
        if (itemIndex < 0)
            return false;

        RemoveAt(itemIndex);
        return true;
    }

    public bool Remove(Guid guid)
    {
        return _registrations.Remove(guid);
    }

    public void RemoveAt(int index)
    {
        _registrations.RemoveAt(index);
    }
}
