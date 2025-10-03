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

/// <summary>
/// Represents a handle for UI action bindings used within the Common UI framework.
/// This struct provides functionality to manage action bindings and their associated properties.
/// </summary>
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

    /// <summary>
    /// Indicates whether the current action binding handle is valid.
    /// This property checks the internal state of the handle to determine if it references a valid action binding.
    /// </summary>
    public bool IsValid => this.IsValid();

    /// <summary>
    /// Unregisters the UI action binding associated with this handle.
    /// This effectively disables the action binding, making it no longer active or functional.
    /// </summary>
    /// <remarks>
    /// Once this method is called, the action binding will be unregistered and cannot be used until it is re-registered.
    /// Ensure that the handle is valid before calling this method to avoid unintended behavior.
    /// </remarks>
    public void Unregister()
    {
        UUIActionBindingExtensions.Unregister(this);
    }

    /// <summary>
    /// Resets the hold functionality associated with this UI action binding handle.
    /// This typically resets the internal state of the action, making it ready to be used again.
    /// </summary>
    /// <remarks>
    /// Use this method to clear or reinitialize any temporary hold states tied to the action binding.
    /// This is especially useful in cases where the action needs to be reused or refreshed after being held.
    /// Ensure the handle is valid before invoking this method to avoid unexpected behavior.
    /// </remarks>
    public void ResetHold()
    {
        UUIActionBindingExtensions.ResetHold(this);
    }

    /// <summary>
    /// Gets the name of the action associated with this UI action binding handle.
    /// This property retrieves the identifier of the action that the handle is bound to in the Common UI framework.
    /// </summary>
    public FName ActionName => this.GetActionName();

    /// <summary>
    /// Gets or sets the display name associated with the action binding.
    /// This property provides a user-friendly text that represents the name of the action binding within the UI.
    /// </summary>
    public FText DisplayName
    {
        get => this.GetDisplayName();
        set => UUIActionBindingExtensions.SetDisplayName(this, value);
    }

    /// <summary>
    /// Determines whether the associated action binding should be displayed in the action bar.
    /// This property allows toggling the visibility of the action binding in the UI's action bar.
    /// </summary>
    public bool DisplayInActionBar
    {
        get => this.GetDisplayInActionBar();
        set => UUIActionBindingExtensions.SetDisplayInActionBar(this, value);
    }

    /// <summary>
    /// Gets the widget bound to this action binding.
    /// This property retrieves the associated UI widget, if any, that corresponds to the current action binding handle.
    /// </summary>
    public UWidget? BoundWidget => this.GetBoundWidget();

    /// <summary>
    /// Retrieves the local player associated with the current action binding.
    /// This property provides access to the local player bound to the UI action,
    /// allowing interaction with the player's context or state.
    /// </summary>
    public ULocalPlayer? BoundLocalPlayer => this.GetBoundLocalPlayer();

    public FUIActionBindingHandle(IntPtr buffer)
    {
        unsafe
        {
            var nativeHandle = (FNativeUIActionBindingHandle*)buffer;
#if !UE_BUILD_SHIPPING
            _cachedDebugActionName = StringMarshaller.FromNative((IntPtr)(&nativeHandle->CachedDebugActionName), 0);
#endif
            _registrationId = nativeHandle->RegistrationId;
        }
    }

    private static readonly IntPtr NativeClassPtr = UCoreUObjectExporter.CallGetNativeStructFromName(
        typeof(FUIActionBindingHandle).GetAssemblyName(),
        "UnrealSharp.CommonUI",
        "UIActionBindingHandle"
    );

    /// <inheritdoc />
    public static IntPtr GetNativeClassPtr() => NativeClassPtr;

    /// <summary>
    /// Represents the size, in bytes, of the native data structure for the UI action binding handle.
    /// This value is used to determine the memory footprint of the native counterpart of the structure
    /// within the Unreal Engine runtime.
    /// </summary>
    public static readonly int NativeDataSize = UScriptStructExporter.CallGetNativeStructSize(NativeClassPtr);

    /// <inheritdoc />
    public static int GetNativeDataSize() => NativeDataSize;

    /// <inheritdoc />
    public static FUIActionBindingHandle FromNative(IntPtr buffer)
    {
        return new FUIActionBindingHandle(buffer);
    }

    /// <inheritdoc />
    public void ToNative(IntPtr buffer)
    {
        unsafe
        {
            var nativeHandle = (FNativeUIActionBindingHandle*)buffer;
#if !UE_BUILD_SHIPPING
            StringMarshaller.ToNative((IntPtr)(&nativeHandle->CachedDebugActionName), 0, _cachedDebugActionName);
#endif
            nativeHandle->RegistrationId = _registrationId;
        }
    }
}
