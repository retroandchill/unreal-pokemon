#if UNREAL_INJECT_GENERATOR
using RhoMicro.CodeAnalysis;
#endif

namespace UnrealInject.SourceGenerator.Attributes;

[AttributeUsage(AttributeTargets.Class)]
#if UNREAL_INJECT_GENERATOR
[IncludeFile]
#endif
internal class AutowiredAttribute : Attribute;

[AttributeUsage(AttributeTargets.Method)]
internal class AutowireInitializerAttribute : Attribute;
