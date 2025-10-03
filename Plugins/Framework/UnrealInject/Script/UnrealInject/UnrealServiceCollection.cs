using System.Collections;
using Microsoft.Extensions.DependencyInjection;

namespace UnrealInject;

/// <summary>
/// Represents a custom implementation of the <see cref="IServiceCollection"/> interface.
/// The UnrealServiceCollection is used to manage service descriptors in a
/// structured and ordered way, providing support for managing and manipulating services
/// through custom methods and functionalities.
/// This collection stores service descriptors internally using an ordered dictionary
/// with a unique <see cref="Guid"/> as the key, ensuring deterministic and fast retrieval
/// of services. It is primarily designed for scenarios where advanced service registration
/// and manipulation are required.
/// The class supports adding, removing, indexing, and enumerating service descriptors,
/// along with operations tailored to provide unique identifiers for services.
/// This class is useful in advanced dependency injection scenarios, where precise
/// control over service registrations and lifetime management is required.
/// Implements the <see cref="IServiceCollection"/> interface.
/// </summary>
public class UnrealServiceCollection : IServiceCollection
{
    private readonly OrderedDictionary<Guid, ServiceDescriptor> _registrations = new();

    /// <inheritdoc />
    public int Count => _registrations.Count;

    /// <inheritdoc />
    public bool IsReadOnly => false;

    /// <inheritdoc />
    public ServiceDescriptor this[int index]
    {
        get => _registrations.GetAt(index).Value;
        set => _registrations.SetAt(index, value);
    }

    /// <summary>
    /// Indexer that allows getting or setting a <see cref="ServiceDescriptor"/> associated with the specified <see cref="Guid"/> key.
    /// Provides direct access to the internal collection using the unique identifier for efficient retrieval and modification.
    /// Throws a <see cref="KeyNotFoundException"/> if the specified key does not exist in the collection during retrieval.
    /// Setting a value with a <see cref="Guid"/> will overwrite the existing descriptor if it already exists with the same key.
    /// </summary>
    /// <param name="key">The unique <see cref="Guid"/> representing the key of the service descriptor.</param>
    /// <returns>The <see cref="ServiceDescriptor"/> associated with the specified <paramref name="key"/>.</returns>
    public ServiceDescriptor this[Guid key]
    {
        get => _registrations[key];
        set => _registrations[key] = value;
    }

    /// <inheritdoc />
    public IEnumerator<ServiceDescriptor> GetEnumerator()
    {
        return _registrations.Values.GetEnumerator();
    }

    IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();

    void ICollection<ServiceDescriptor>.Add(ServiceDescriptor item)
    {
        Add(item);
    }

    /// <summary>
    /// Adds a new service descriptor to the collection and assigns it a unique <see cref="Guid"/>.
    /// </summary>
    /// <param name="item">The service descriptor to add.</param>
    /// <returns>The unique <see cref="Guid"/> associated with the added service.</returns>
    public Guid Add(ServiceDescriptor item)
    {
        var guid = Guid.CreateVersion7();
        _registrations.Add(guid, item);
        return guid;
    }

    /// <inheritdoc />
    public void Clear()
    {
        _registrations.Clear();
    }

    /// <inheritdoc />
    public bool Contains(ServiceDescriptor item)
    {
        return _registrations.ContainsValue(item);
    }

    /// <inheritdoc />
    public void CopyTo(ServiceDescriptor[] array, int arrayIndex)
    {
        _registrations.Values.CopyTo(array, arrayIndex);
    }

    /// <inheritdoc />
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

    /// <summary>
    /// Inserts a specified <see cref="ServiceDescriptor"/> at the specified index and associates it with a unique <see cref="Guid"/>.
    /// </summary>
    /// <param name="index">The zero-based index at which the <see cref="ServiceDescriptor"/> should be inserted.</param>
    /// <param name="item">The <see cref="ServiceDescriptor"/> to insert into the collection.</param>
    /// <returns>A <see cref="Guid"/> uniquely associated with the inserted <see cref="ServiceDescriptor"/>.</returns>
    public Guid Insert(int index, ServiceDescriptor item)
    {
        var guid = Guid.NewGuid();
        _registrations.Insert(index, guid, item);
        return guid;
    }

    /// <inheritdoc />
    public bool Remove(ServiceDescriptor item)
    {
        var itemIndex = IndexOf(item);
        if (itemIndex < 0)
            return false;

        RemoveAt(itemIndex);
        return true;
    }

    /// <summary>
    /// Removes the service descriptor associated with the specified unique identifier.
    /// </summary>
    /// <param name="guid">The unique identifier of the service descriptor to remove.</param>
    /// <returns>
    /// <c>true</c> if the service descriptor was successfully removed; otherwise, <c>false</c>.
    /// </returns>
    public bool Remove(Guid guid)
    {
        return _registrations.Remove(guid);
    }

    /// <inheritdoc />
    public void RemoveAt(int index)
    {
        _registrations.RemoveAt(index);
    }
}
