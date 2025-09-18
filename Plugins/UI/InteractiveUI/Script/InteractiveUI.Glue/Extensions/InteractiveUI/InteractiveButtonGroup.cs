namespace UnrealSharp.InteractiveUI;

public partial class UInteractiveButtonGroup
{
    public event AddButton OnButtonAdded
    {
        add => OnAddButtonDelegate += value;
        remove => OnAddButtonDelegate -= value;
    }

    public event RemoveButton OnButtonRemoved
    {
        add => OnRemoveButtonDelegate += value;
        remove => OnRemoveButtonDelegate -= value;
    }
}
