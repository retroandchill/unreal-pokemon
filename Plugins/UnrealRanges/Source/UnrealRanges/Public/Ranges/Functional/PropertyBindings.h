// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

#if WITH_EDITOR
/**
 * Helper macro for binding to a delegate or using the constant value when constructing the underlying SWidget.
 * These macros create a binding that has a layer of indirection that allows blueprint debugging to work more effectively.
 */
#define PROPERTY_BINDING_WRAPPED(ReturnType, MemberName)					\
( MemberName ## Delegate.IsBound() && !IsDesignTime() )			\
?																\
BIND_UOBJECT_ATTRIBUTE(ReturnType, K2_Gate_ ## MemberName)	\
:																\
TAttribute< ReturnType >(Get##MemberName())

#define BITFIELD_PROPERTY_BINDING_WRAPPED(MemberName)						\
( MemberName ## Delegate.IsBound() && !IsDesignTime() )			\
?																\
BIND_UOBJECT_ATTRIBUTE(bool, K2_Gate_ ## MemberName)		\
:																\
TAttribute< bool >(Get##MemberName() != 0)

#else

#define PROPERTY_BINDING_WRAPPED(ReturnType, MemberName)				\
( MemberName ## Delegate.IsBound() && !IsDesignTime() )		\
?															\
TAttribute< ReturnType >::Create(MemberName ## Delegate.GetUObject(), MemberName ## Delegate.GetFunctionName()) \
:															\
TAttribute< ReturnType >(Get##MemberName())

#define BITFIELD_PROPERTY_BINDING_WRAPPED(MemberName)					\
( MemberName ## Delegate.IsBound() && !IsDesignTime() )		\
?															\
TAttribute< bool >::Create(MemberName ## Delegate.GetUObject(), MemberName ## Delegate.GetFunctionName()) \
:															\
TAttribute< bool >(Get##MemberName() != 0)

#endif
