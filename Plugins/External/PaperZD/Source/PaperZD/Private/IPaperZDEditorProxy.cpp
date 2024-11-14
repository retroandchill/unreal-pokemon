// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "IPaperZDEditorProxy.h"

//Static definition
TSharedPtr<IPaperZDEditorProxy> IPaperZDEditorProxy::EditorProxy = nullptr;

//Instance get/set
void IPaperZDEditorProxy::SetEditorProxy(TSharedPtr<IPaperZDEditorProxy> InEditorProxy)
{
	EditorProxy = InEditorProxy;
}

TSharedPtr<IPaperZDEditorProxy> IPaperZDEditorProxy::Get()
{
	return EditorProxy;
}
