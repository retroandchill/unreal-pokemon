namespace UnrealSharp.GameDataAccessTools;

internal readonly ref partial struct FGameplayTagContainerView(IntPtr ptr)
{
    private readonly IntPtr _ptr = ptr;

    public static implicit operator FGameplayTagContainerView(IntPtr ptr) => new(ptr);
}
