// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "CoreMinimal.h"

class FPaperZDAnimBPCompilerAccess;

//typedef the unique class identifier type
#if PLATFORM_MAC
//size_t is not hashable, we use the default uint64 one (which is most probably the same as size_t on other platforms as well)
typedef uint64 zid_t;
#else
//default implementation is more portable and thus we prefer it
typedef size_t zid_t;
#endif

/**
 * Helper class for giving an unique identifier to each class that inherits from this.
 * Objects can be keyed by calling T::GetClassId()
 */
template <typename T>
class TUniqueClassIdentifier
{
	/* Dummy variable that gets created per-class, so we can request an unique identifier. */
	static const uint8 __Dummy;

public:
	/* Get a class unique identifier. */
	static zid_t GetClassId() { return reinterpret_cast<zid_t>(&__Dummy); }
};

 template <typename T>
 const uint8 TUniqueClassIdentifier<T>::__Dummy = 0;

/**
 * Compiler handle that can be created to process certain animation nodes that would require more specific processing.
 * Stored in the compiler on a per-instance basis.
 * Child class must inherit from TUniqueClassIdentifier<T> for registration purposes.
 */
class IPaperZDAnimBPCompilerHandle
{
public:
	virtual ~IPaperZDAnimBPCompilerHandle() {}

	/* Initialize this instance with the specified compiler. */
	virtual void Initialize(FPaperZDAnimBPCompilerAccess& InCompilerAccess) = 0;
};

/**
 * Factory object used to create a specific compiler handle.
 * Gets registered globally on the plugin and gets called whenever a compiler context requires to generate its handles.
 * Child class must inherit from TUniqueClassIdentifier<T> for registration purposes.
 */
class IPaperZDCompilerHandleFactory
{
public:
	virtual ~IPaperZDCompilerHandleFactory()
	{}

	/* Creates a new handle and returns its ClassId as well.  */
	virtual IPaperZDAnimBPCompilerHandle* CreateNew(FPaperZDAnimBPCompilerAccess& CompilerAccess, zid_t& HandleId) = 0;
 };


 /**
  * Helper templated handle factory to quickly create a simple factory for a compiler handle
  */
template <typename T>
class TPaperZDCompilerHandleFactory : public IPaperZDCompilerHandleFactory, public TUniqueClassIdentifier<T>
{
	/* Creates a new compiler handler. */
	IPaperZDAnimBPCompilerHandle* CreateNewImpl(FPaperZDAnimBPCompilerAccess& CompilerAccess, zid_t& HandleId)
	{
		//Create the class
		IPaperZDAnimBPCompilerHandle* NewHandle = new T();
		NewHandle->Initialize(CompilerAccess);

		//Register the handle
		HandleId = T::GetClassId();

		return NewHandle;
	}

public:
	//~Begin IPaperZDCompilerHandleFactory Interface
	virtual IPaperZDAnimBPCompilerHandle* CreateNew(FPaperZDAnimBPCompilerAccess& CompilerAccess, zid_t& HandleId) override
	{
		return CreateNewImpl(CompilerAccess, HandleId);
	}
	//~End IPaperZDCompilerHandleFactory Interface
};
