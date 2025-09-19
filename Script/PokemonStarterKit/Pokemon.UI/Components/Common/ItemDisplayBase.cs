using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;
using UnrealSharp.CommonUI;

namespace Pokemon.UI.Components.Common;

[UClass(ClassFlags.Abstract)]
public class UItemDisplayBase : UCommonUserWidget
{
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Content")]
    public FItemHandle Item
    {
        get;
        set
        {
            if (field == value) return;
            
            field = value;
            Refresh();
        } 
    }
    
    [UFunction(FunctionFlags.BlueprintCallable | FunctionFlags.BlueprintEvent, Category = "Display")]
    public virtual void Refresh()
    {
        
    }
}