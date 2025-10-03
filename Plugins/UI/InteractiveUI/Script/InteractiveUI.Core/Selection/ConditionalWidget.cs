using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CommonUI;
using UnrealSharp.UMG;

namespace InteractiveUI.Core.Selection;

/// <summary>
/// Represents a conditional widget interface that defines the ability to enable or disable
/// a widget based on specified conditions.
/// </summary>
public interface IConditionalWidget
{
    /// <summary>
    /// Gets a value indicating whether the widget is currently enabled.
    /// If enabled, the widget can interact with user inputs or system behaviors.
    /// </summary>
    /// <remarks>
    /// This property is typically used to control the interactive state of a widget
    /// based on specific conditions or business logic. The value of <see cref="IsWidgetEnabled" />
    /// may be updated dynamically through methods such as <see cref="UpdateEnabled" />.
    /// If the widget is disabled, it may not be visible or interactable depending on its implementation.
    /// </remarks>
    bool IsWidgetEnabled { get; }

    /// <summary>
    /// Updates the enabled state of the widget based on the associated delegate or predefined conditions.
    /// </summary>
    /// <remarks>
    /// This method evaluates the current conditions tied to the widget and updates its state to either enabled or disabled.
    /// If a delegate is bound to determine the enabled state, it will be invoked to set the value of <see cref="IConditionalWidget.IsWidgetEnabled"/>.
    /// Additionally, the widget's visibility is modified to match the corresponding state, using predefined visibility settings for enabled and disabled states.
    /// </remarks>
    void UpdateEnabled();
}

/// <summary>
/// Defines whether the widget is enabled based on certain conditions.
/// </summary>
[USingleDelegate]
public delegate bool IsWidgetEnabled();

/// <summary>
/// Represents a conditional button base that inherits from the common button base and implements
/// conditional behavior for enabling or disabling the button based on specified conditions.
/// </summary>
[UClass(ClassFlags.Abstract)]
public class UConditionalButtonBase : UCommonButtonBase, IConditionalWidget
{
    /// <summary>
    /// Gets a value that determines whether the widget is enabled.
    /// When enabled, the widget can be interacted with in the user interface and may respond to user actions or system updates.
    /// </summary>
    /// <remarks>
    /// This property is commonly utilized to reflect or modify the active state of a widget in response to underlying conditions.
    /// It may be updated programmatically, often during operations such as <see cref="UpdateEnabled"/>, where the state of the widget is reassessed.
    /// If the widget is disabled, its visual or interactive state may change, depending on the implementation.
    /// </remarks>
    [UProperty(PropertyFlags.EditAnywhere, Category = "Selection")]
    public bool IsWidgetEnabled { get; private set; } = true;

    [UProperty]
    private TDelegate<IsWidgetEnabled> IsWidgetEnabledDelegate { get; }

    /// <summary>
    /// Gets or sets the visibility of the widget when it is in the enabled state.
    /// </summary>
    /// <remarks>
    /// This property determines how the widget appears when it is enabled. The possible values are defined
    /// by the <see cref="ESlateVisibility"/> enumeration. Invalid options for this property include
    /// "Collapsed" and "Hidden," ensuring the widget remains at least visually interactable when enabled.
    /// The value of <see cref="EnabledVisibility"/> is used in conjunction with <see cref="DisabledVisibility"/>
    /// to dynamically update the widget's visibility based on its enabled state, which is determined by
    /// methods like <see cref="UpdateEnabled"/>.
    /// </remarks>
    [UProperty(PropertyFlags.EditAnywhere, Category = "Visibility")]
    [UMetaData("InvalidEnumValues", "Collapsed, Hidden")]
    private ESlateVisibility EnabledVisibility { get; set; } = ESlateVisibility.SelfHitTestInvisible;

    /// <summary>
    /// Gets or sets the visibility of the widget when it is in the disabled state.
    /// </summary>
    /// <remarks>
    /// This property determines how the widget appears when it is disabled. The possible values are defined
    /// by the <see cref="ESlateVisibility"/> enumeration. Invalid options for this property include
    /// "Collapsed" and "Hidden," ensuring that the widget does not completely disappear or lose all interactivity
    /// in a disabled state if such behavior is not desired.
    /// The value of <see cref="DisabledVisibility"/> is used together with <see cref="EnabledVisibility"/>
    /// to dynamically update the widget's visibility based on its enabled state. Changes to this property
    /// will typically be reflected when invoking methods like <see cref="UpdateEnabled"/>.
    /// </remarks>
    [UProperty(PropertyFlags.EditAnywhere, Category = "Visibility")]
    [UMetaData("InvalidEnumValues", "Collapsed, Hidden")]
    private ESlateVisibility DisabledVisibility { get; set; } = ESlateVisibility.Collapsed;

    /// <inheritdoc />
    public void UpdateEnabled()
    {
        if (IsWidgetEnabledDelegate.IsBound)
        {
            IsWidgetEnabled = IsWidgetEnabledDelegate.InnerDelegate.Invoke();
        }

        Visibility = IsWidgetEnabled ? EnabledVisibility : DisabledVisibility;
    }
}

/// <summary>
/// Represents a base class for text-based conditional buttons, providing functionality to update
/// its enabled state and visibility based on specified conditions.
/// </summary>
[UClass(ClassFlags.Abstract)]
public class UConditionalTextButtonBase : USimpleTextButtonBase, IConditionalWidget
{
    /// <summary>
    /// Determines whether the widget is enabled and capable of interacting with user inputs or system behaviors.
    /// </summary>
    /// <remarks>
    /// The value of this property reflects the current state of the widget and is influenced by conditions
    /// or delegates that dynamically control its enabled state. If the widget's state is updated using methods
    /// such as <see cref="UpdateEnabled" />, this property will reflect the new enabled or disabled state.
    /// A disabled widget may also alter its visibility depending on the specific implementation of the widget.
    /// </remarks>
    [UProperty(PropertyFlags.EditAnywhere, Category = "Selection")]
    public bool IsWidgetEnabled { get; private set; } = true;

    [UProperty]
    private TDelegate<IsWidgetEnabled> IsWidgetEnabledDelegate { get; }

    /// <summary>
    /// Gets or sets the visibility state of the widget when it is enabled.
    /// Determines how the widget will be presented in the user interface when it is actively enabled.
    /// </summary>
    /// <remarks>
    /// This property is used to define the specific visibility behavior of the widget when it is in an enabled state.
    /// It is typically utilized in conjunction with the <see cref="IConditionalWidget.IsWidgetEnabled" /> implementation
    /// to provide dynamic UI updates. The value of <see cref="EnabledVisibility" /> should be set to a valid
    /// <see cref="ESlateVisibility" /> state, excluding "Collapsed" or "Hidden", to ensure the widget remains interactable
    /// and visible when enabled.
    /// </remarks>
    [UProperty(PropertyFlags.EditAnywhere, Category = "Visibility")]
    [UMetaData("InvalidEnumValues", "Collapsed, Hidden")]
    private ESlateVisibility EnabledVisibility { get; set; } = ESlateVisibility.SelfHitTestInvisible;

    /// <summary>
    /// Gets or sets the visibility state of the widget when it is disabled.
    /// </summary>
    /// <remarks>
    /// This property determines how the widget appears in the user interface when it is not enabled.
    /// The assigned value must comply with the expected visibility states, excluding invalid enum values
    /// such as "Collapsed" or "Hidden" as specified by metadata. The visibility of the widget can dynamically
    /// change based on its enabled or disabled state, which is updated through the <see cref="UpdateEnabled" /> method.
    /// </remarks>
    [UProperty(PropertyFlags.EditAnywhere, Category = "Visibility")]
    [UMetaData("InvalidEnumValues", "Collapsed, Hidden")]
    private ESlateVisibility DisabledVisibility { get; set; } = ESlateVisibility.Collapsed;

    /// <inheritdoc />
    public void UpdateEnabled()
    {
        if (IsWidgetEnabledDelegate.IsBound)
        {
            IsWidgetEnabled = IsWidgetEnabledDelegate.InnerDelegate.Invoke();
        }

        Visibility = IsWidgetEnabled ? EnabledVisibility : DisabledVisibility;
    }
}
