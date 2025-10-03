using System.Diagnostics.CodeAnalysis;
using InteractiveUI.Core.Selection;
using InteractiveUI.Core.Utilities;
using Pokemon.Core;
using Pokemon.Core.Entities;
using Pokemon.Data;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.EnhancedInput;
using UnrealSharp.GameplayTags;
using UnrealSharp.InteractiveUI;
using UnrealSharp.UnrealSharpCommonUI;

namespace Pokemon.UI.Components.Bag;

/// <summary>
/// Represents a tab widget for managing and selecting pockets within the bag system of the UI in a Pokemon-based application.
/// </summary>
/// <remarks>
/// This class inherits from <see cref="UCommonActivatableWidget"/> and provides functionality for handling pocket tabs in the user interface. It includes support for UI action binding, pocket selection, and event notifications when the selected pocket changes.
/// </remarks>
[UClass]
public class UPocketTabWidget : UCommonActivatableWidget
{
    /// <summary>
    /// Gets or sets the currently selected pocket within the pocket tab widget.
    /// </summary>
    /// <remarks>
    /// When the value of <c>CurrentPocket</c> is changed, the <c>OnPocketChanged</c> event is triggered, allowing
    /// other components to respond to the pocket selection change. It is primarily used to synchronize the selected
    /// pocket across the user interface.
    /// Setting this property also updates the <c>Bag.LastViewedPocket</c> to reflect the new selection. This property is
    /// designed to only be set through internal logic and is marked as <c>private set</c>.
    /// </remarks>
    public FGameplayTag CurrentPocket
    {
        get;
        private set
        {
            if (field == value)
                return;
            field = value;

            Bag.LastViewedPocket = value;
            OnPocketChanged?.Invoke(value);
        }
    }

    [UProperty(PropertyFlags.Transient)]
    private UPokemonBag Bag { get; set; }

    /// <summary>
    /// Event that is triggered whenever the currently selected pocket changes in the pocket tab widget.
    /// </summary>
    /// <remarks>
    /// This event notifies subscribers of changes to the selected pocket by invoking all registered event handlers and passing the new pocket as a parameter.
    /// It is used to synchronize changes across different components of the UI, ensuring that updates to the selected pocket are reflected in related systems such as the bag screen or item selection window.
    /// Handlers for this event may include actions such as updating visuals, changing inventory displays, or modifying associated data.
    /// </remarks>
    public event Action<FGameplayTag>? OnPocketChanged;

    [UProperty(PropertyFlags.Transient)]
    private USelectableButtonGroup PocketButtonGroup { get; set; }

    /// <summary>
    /// Specifies the class type for the button widget associated with pockets in the pocket tab system.
    /// </summary>
    /// <remarks>
    /// The <c>ButtonClass</c> property defines the button's widget class used to dynamically create pocket buttons
    /// within the UI. It is primarily utilized during the construction and initialization of the pocket tab widget.
    /// The value of this property must be a subclass of <c>UPocketButton</c> and is essential for ensuring proper
    /// button instantiation and styling in the UI.
    /// When a button is created, it inherits properties and styles defined in the specified button class. This property
    /// must be assigned with a valid button class during design or initialization to enable functionality.
    /// </remarks>
    [UProperty(PropertyFlags.EditAnywhere, Category = "Buttons")]
    private TSubclassOf<UPocketButton> ButtonClass { get; }

    /// <summary>
    /// Maps pocket types represented as gameplay tags to their corresponding button styles.
    /// </summary>
    /// <remarks>
    /// The <c>PocketButtonStyles</c> property defines a relationship between pocket identifiers
    /// and specific button appearance styles. It is used to configure the visual representation
    /// of button elements within the pocket tab interface.
    /// Each entry in the map pairs a <c>FGameplayTag</c> with a <c>TSubclassOf<UCommonButtonStyle></c>,
    /// specifying the appropriate button style to be applied for the given pocket type.
    /// This property is editable in the Unreal Editor and allows designers to assign
    /// predefined button visual styles to match pocket types. It supports fixed size entries
    /// and aligns with specific UI customization requirements.
    /// </remarks>
    [UProperty(PropertyFlags.EditAnywhere | PropertyFlags.EditFixedSize, Category = "Buttons")]
    [Categories(IdentifierConstants.PocketTag)]
    private TMap<FGameplayTag, TSubclassOf<UCommonButtonStyle>> PocketButtonStyles { get; }

    /// <summary>
    /// Gets or sets the input action used to navigate to the left pocket within the pocket tab widget.
    /// </summary>
    /// <remarks>
    /// <c>PocketLeftAction</c> is bound to the enhanced input system and enables user interaction specifically
    /// for selecting the previous pocket in the sequence. This property is primarily configured internally during the
    /// widget's construction phase to associate the necessary input behavior for the left navigation action.
    /// It is leveraged to ensure consistent and responsive UI interactions for pocket navigation.
    /// </remarks>
    [UProperty(PropertyFlags.EditAnywhere, Category = "Input")]
    private UInputAction PocketLeftAction { get; set; }

    /// <summary>
    /// Gets or sets the input action associated with navigating to the next pocket in the pocket tab widget.
    /// </summary>
    /// <remarks>
    /// The <c>PocketRightAction</c> is bound to an input action that enables users to navigate to the next pocket
    /// within the pocket tab view. This property is utilized internally when setting up enhanced input actions
    /// and is assigned to the appropriate widget or handler responsible for user interaction.
    /// Typically, this action is used in conjunction with <c>PocketLeftAction</c> to provide seamless navigation
    /// between pockets.
    /// </remarks>
    [UProperty(PropertyFlags.EditAnywhere, Category = "Input")]
    private UInputAction PocketRightAction { get; set; }

    [UProperty]
    [BindWidget]
    private UCommonActionWidget PocketLeftActionWidget { get; }

    [UProperty]
    [BindWidget]
    private UCommonActionWidget PocketRightActionWidget { get; }

    /// <summary>
    /// Represents a tab widget for managing and interacting with pockets within the bag system in the UI.
    /// </summary>
    /// <remarks>
    /// This class extends <see cref="UCommonActivatableWidget"/>, integrating pocket-related functionality into the user interface.
    /// It utilizes settings from <see cref="UPokemonCoreSettings"/> to initialize pocket button styles and manages user action bindings.
    /// </remarks>
    public UPocketTabWidget()
    {
        var settings = GetDefault<UPokemonCoreSettings>();
        foreach (var (tag, _, _) in settings.Pockets)
        {
            PocketButtonStyles!.Add(tag, default);
        }
    }

    /// <inheritdoc />
    public override void PreConstruct(bool isDesignTime)
    {
        if (isDesignTime)
        {
            if (!ButtonClass.Valid)
                return;
        }

        // ReSharper disable once ConditionalAccessQualifierIsNonNullableAccordingToAPIContract
        PocketButtonGroup?.RemoveAll();

        PocketButtonGroup = NewObject<USelectableButtonGroup>(this);
        PocketButtonGroup.OnButtonAdded += [UFunction]
        (_, b) =>
        {
            SlotButton(b);
        };
        PocketButtonGroup.OnButtonRemoved += [UFunction]
        (b) =>
        {
            b.RemoveFromParent();
        };
        PocketButtonGroup.OnSelectedButtonBaseChanged += [UFunction]
        (b, i) =>
        {
            var button = (UPocketButton)b;
            CurrentPocket = button.Pocket;
        };
        PocketButtonGroup.OnButtonBaseClicked += PocketClicked;

        foreach (var (tag, _, _) in GetDefault<UPokemonCoreSettings>().Pockets)
        {
            var pocketButton = WidgetTree.ConstructWidget(ButtonClass);
            pocketButton.Pocket = tag;
            pocketButton.Style = PocketButtonStyles[tag];
            PocketButtonGroup.AddWidget(pocketButton);
        }

        PocketLeftActionWidget.EnhancedInputAction = PocketLeftAction;
        PocketRightActionWidget.EnhancedInputAction = PocketRightAction;
    }

    /// <inheritdoc />
    public override void Construct()
    {
        RegisterUIActionBinding(new FCSBindUIActionArgs(PocketLeftAction, false, PocketLeft));
        RegisterUIActionBinding(new FCSBindUIActionArgs(PocketRightAction, false, PocketRight));

        var subsystem = GetGameInstanceSubsystem<UPokemonSubsystem>();
        Bag = subsystem.Bag;
        var lastPocket = Bag.LastViewedPocket;
        if (lastPocket.IsValid)
        {
            CurrentPocket = lastPocket;
            var matchingButton = PocketButtonGroup
                .GetButtons<UPocketButton>()
                .Where(x => x.Button.Pocket == lastPocket)
                .Select(x => x.Button)
                .First();
            matchingButton.SetIsSelected(true);
        }
        else
        {
            var firstButton = PocketButtonGroup.GetRequiredButton<UPocketButton>(0);
            CurrentPocket = firstButton.Pocket;
        }
    }

    /// <summary>
    /// Assigns and handles the functionality associated with a slot button within the pocket tab widget.
    /// </summary>
    /// <param name="button">The button instance of type <see cref="UCommonButtonBase"/> that is being utilized to represent a specific slot.</param>
    /// <remarks>
    /// This method is a virtual placeholder, intended for Blueprint or derived class implementation. It serves to link a slot button to its corresponding functionality or visual behavior within the context of the pocket tab widget interface.
    /// </remarks>
    [UFunction(FunctionFlags.BlueprintEvent, Category = "Buttons")]
    protected virtual void SlotButton(UCommonButtonBase button)
    {
        // No native implementation
    }

    [UFunction]
    private void PocketLeft()
    {
        PocketButtonGroup.SelectPreviousButton();
    }

    [UFunction]
    private void PocketRight()
    {
        PocketButtonGroup.SelectNextButton();
    }

    [UFunction]
    private void PocketClicked(UCommonButtonBase button, int index)
    {
        var pocketButton = (UPocketButton)button;
        pocketButton.SetIsSelected(true);
        CurrentPocket = pocketButton.Pocket;
    }
}
