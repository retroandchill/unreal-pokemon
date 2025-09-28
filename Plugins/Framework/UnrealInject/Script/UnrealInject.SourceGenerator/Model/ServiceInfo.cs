using Retro.SourceGeneratorUtilities.Utilities.Attributes;
using UnrealInject.SourceGenerator.Attributes;

namespace UnrealInject.SourceGenerator.Model;

[AttributeInfoType<ServiceAttribute>]
public readonly record struct ServiceInfo(ServiceLifetime ServiceLifetime);
