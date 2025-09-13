namespace UnrealInject.Options;

public interface IOptionsConfiguration<in T>
{
    void Configure(T options);
}
