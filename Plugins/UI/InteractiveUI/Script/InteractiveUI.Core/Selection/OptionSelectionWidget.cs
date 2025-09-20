using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.UnrealSharpCommonUI;

namespace InteractiveUI.Core.Selection;

[UClass(ClassFlags.Abstract)]
public class UOptionSelectionWidget : UOwningSelectionWidget
{
    [UProperty(PropertyFlags.EditAnywhere, Category = "Options")]
    public TSubclassOf<USimpleTextButtonBase> ButtonClass { get; }

    [UProperty(PropertyFlags.EditAnywhere, Category = "Options")]
    public TArray<FText> Options { get; }

    [UProperty(PropertyFlags.EditAnywhere, Category = "Options")]
    public int? CancelOptionIndex { get; private set; }

    public event Action<int>? OnOptionSelected;

    public override void Construct()
    {
        base.Construct();
        Buttons.OnButtonBaseClicked += [UFunction] (_, i) => OnOptionSelected?.Invoke(i);
    }

    public void SetOptions(IEnumerable<FText> options, int? cancelOptionIndex = null)
    {
        Options.Clear();
        foreach (var option in options)
        {
            Options.Add(option);
        }

        CancelOptionIndex =
            cancelOptionIndex is >= 0 && cancelOptionIndex.Value < Options.Count
                ? cancelOptionIndex.Value
                : null;
    }

    private void CreateOptions()
    {
        Buttons.RemoveAll();
        foreach (var (i, option) in Options.Index())
        {
            var newButton = WidgetTree.ConstructWidget(ButtonClass);
            newButton.Text = option;

            if (i == CancelOptionIndex)
            {
                newButton.TriggeringEnhancedInputAction =
                    UCommonInputExtensions.EnhancedInputBackAction;
            }

            Buttons.AddWidget(newButton);
        }
    }
}
