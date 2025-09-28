using Microsoft.Extensions.DependencyInjection;
using Retro.SourceGeneratorUtilities.Utilities.Attributes;
using UnrealInject.SourceGenerator.Attributes;

namespace UnrealInject.SourceGenerator.Model;

[AttributeInfoType<ServiceAttribute>]
internal readonly record struct ServiceInfo(ServiceLifetime ServiceLifetime);
