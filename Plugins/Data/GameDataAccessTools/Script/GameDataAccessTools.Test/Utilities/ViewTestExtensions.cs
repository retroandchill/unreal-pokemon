using System.Linq.Expressions;
using GameDataAccessTools.Core.Marshallers;
using GameDataAccessTools.Core.Views;
using GameDataAccessTools.Test.Views;
using UnrealSharp;
using UnrealSharp.CoreUObject;
using UnrealSharp.Interop;

namespace GameDataAccessTools.Test.Utilities;

public static class ViewTestExtensions
{
    public static StructView<TStruct> GetView<TObject, TStruct>(
        this TObject instance,
        Expression<Func<TObject, TStruct>> expression
    )
        where TObject : UObject
        where TStruct : struct, MarshalledStruct<TStruct>, allows ref struct
    {
        if (expression.Body is not MemberExpression memberExpression)
        {
            throw new ArgumentException("Expression must be a member access", nameof(expression));
        }

        var propertyName = memberExpression.Member.Name;
        return GetView<TObject, TStruct>(instance, propertyName);
    }

    public static StructView<TStruct> GetView<TObject, TStruct>(this TObject instance, string propertyName)
        where TObject : UObject
        where TStruct : struct, MarshalledStruct<TStruct>, allows ref struct
    {
        var defaultObject = UObject.GetDefault<TObject>();
        var nativeClass = defaultObject.Class.NativeObject;
        var nativeProperty = FPropertyExporter.CallGetNativePropertyFromName(nativeClass, propertyName);
        var offset = FPropertyExporter.CallGetPropertyOffset(nativeProperty);
        return StructViewMarshaller<TStruct>.FromNative(IntPtr.Add(instance.NativeObject, offset), 0);
    }
}
