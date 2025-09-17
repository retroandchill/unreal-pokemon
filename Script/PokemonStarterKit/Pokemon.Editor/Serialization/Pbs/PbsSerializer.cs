namespace Pokemon.Editor.Serialization.Pbs;

public interface IPbsSerializer;

public interface IPbsSerializer<T> : IPbsSerializer
{
    void Serialize(IEnumerable<T> model, TextWriter writer);

    IEnumerable<T> Deserialize(TextReader reader);
}

public class PbsSerializer
{
    private static PbsSerializer? _instance;

    public static PbsSerializer Instance =>
        _instance ?? throw new InvalidOperationException("PbsSerializer is not initialized");

    private readonly Dictionary<Type, IPbsSerializer> _serializers = new();

    public static void Initialize()
    {
        if (_instance is not null)
        {
            throw new InvalidOperationException("PbsSerializer is already initialized");
        }

        _instance = new PbsSerializer();
    }

    public static void Shutdown()
    {
        _instance = null;
    }

    public PbsSerializer RegisterSerializer<T>(IPbsSerializer<T> serializer)
    {
        _serializers.Add(typeof(T), serializer);
        return this;
    }

    public PbsSerializer RemoveSerializer<T>()
    {
        _serializers.Remove(typeof(T));
        return this;
    }

    public bool HasSerializerFor(Type type)
    {
        return _serializers.ContainsKey(type);
    }

    public void Serialize<T>(IEnumerable<T> model, TextWriter writer)
    {
        if (!_serializers.TryGetValue(typeof(T), out var serializer))
        {
            throw new InvalidOperationException($"No serializer for type {typeof(T)}");
        }

        ((IPbsSerializer<T>)serializer).Serialize(model, writer);
    }

    public IEnumerable<T> Deserialize<T>(TextReader reader)
    {
        return _serializers.TryGetValue(typeof(T), out var serializer)
            ? ((IPbsSerializer<T>)serializer).Deserialize(reader)
            : throw new InvalidOperationException($"No serializer for type {typeof(T)}");
    }
}
