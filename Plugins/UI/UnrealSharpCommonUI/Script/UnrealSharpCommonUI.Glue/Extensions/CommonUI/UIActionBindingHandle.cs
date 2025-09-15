using System.Runtime.InteropServices;
using UnrealSharp.Attributes;
using UnrealSharp.Core;
using UnrealSharp.Core.Attributes;
using UnrealSharp.Core.Marshallers;
using UnrealSharp.Engine;
using UnrealSharp.Interop;
using UnrealSharp.UMG;
using UnrealSharp.UnrealSharpCommonUI;

namespace UnrealSharp.CommonUI;

[StructLayout(LayoutKind.Sequential)]
internal struct FNativeUIActionBindingHandle
{
#if !UE_BUILD_SHIPPING
    public UnmanagedArray CachedDebugActionName;
#endif
    public int RegistrationId;
}

[UStruct]
#if !UI_BUILD_SHIPPING
[StructLayout(LayoutKind.Sequential)]
[BlittableType]
#endif
[GeneratedType("UIActionBindingHandle", "UnrealSharp.CommonUI.UIActionBindingHandle")]
public readonly struct FUIActionBindingHandle : MarshalledStruct<FUIActionBindingHandle>
{
#if !UE_BUILD_SHIPPING
    private readonly string _cachedDebugActionName;
#endif
    private readonly int _registrationId;

    public bool IsValid => this.IsValid();

    public void Unregister()
    {
        UUIActionBindingExtensions.Unregister(this);
    }

    public void ResetHold()
    {
        UUIActionBindingExtensions.ResetHold(this);
    }

    public FName ActionName => this.GetActionName();

    public FText DisplayName
    {
        get => this.GetDisplayName();
        set => UUIActionBindingExtensions.SetDisplayName(this, value);
    }

    public bool DisplayInActionBar
    {
        get => this.GetDisplayInActionBar();
        set => UUIActionBindingExtensions.SetDisplayInActionBar(this, value);
    }

    public UWidget? BoundWidget => this.GetBoundWidget();

    public ULocalPlayer? BoundLocalPlayer => this.GetBoundLocalPlayer();

    public FUIActionBindingHandle(IntPtr buffer)
    {
        unsafe
        {
            var nativeHandle = (FNativeUIActionBindingHandle*)buffer;
#if !UE_BUILD_SHIPPING
            _cachedDebugActionName = StringMarshaller.FromNative(
                (IntPtr)(&nativeHandle->CachedDebugActionName),
                0
            );
#endif
            _registrationId = nativeHandle->RegistrationId;
        }
    }

    private static readonly IntPtr NativeClassPtr =
        UCoreUObjectExporter.CallGetNativeStructFromName(
            typeof(FUIActionBindingHandle).GetAssemblyName(),
            "UnrealSharp.CommonUI",
            "UIActionBindingHandle"
        );

    public static IntPtr GetNativeClassPtr() => NativeClassPtr;

    public static readonly int NativeDataSize = UScriptStructExporter.CallGetNativeStructSize(
        NativeClassPtr
    );

    public static int GetNativeDataSize() => NativeDataSize;

    public static FUIActionBindingHandle FromNative(IntPtr buffer)
    {
        return new FUIActionBindingHandle(buffer);
    }

    public void ToNative(IntPtr buffer)
    {
        unsafe
        {
            var nativeHandle = (FNativeUIActionBindingHandle*)buffer;
#if !UE_BUILD_SHIPPING
            StringMarshaller.ToNative(
                (IntPtr)(&nativeHandle->CachedDebugActionName),
                0,
                _cachedDebugActionName
            );
#endif
            nativeHandle->RegistrationId = _registrationId;
        }
    }
}
