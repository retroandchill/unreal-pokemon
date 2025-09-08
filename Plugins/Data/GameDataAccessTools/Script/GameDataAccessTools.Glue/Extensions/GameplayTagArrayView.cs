using UnrealSharp.GameplayTags;

namespace UnrealSharp.GameDataAccessTools;

internal readonly ref partial struct FGameplayTagArrayView(IntPtr ptr)
{
    internal readonly IntPtr Ptr = ptr;

    public static implicit operator FGameplayTagArrayView(IntPtr ptr) => new(ptr);
}
