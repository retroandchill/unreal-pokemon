// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

//Fwd declarations need to be out of the namespace
class SPaperZDAnimAssetBrowser;
class SPaperZDAnimationSourceViewport;
class IDetailsView;
struct FTabId;
struct FAssetData;

namespace FPaperZDEditorDelegates
{
	/* Called when we require an asset to be opened on the editor. */
	DECLARE_DELEGATE_OneParam(OnOpenAssetSignature, const FAssetData&);

	/* Called when we require an asset for asset inspection. */
	DECLARE_DELEGATE_RetVal(UObject*, OnGetAssetSignature);

	/* Called when the asset browser widget gets created. */
	DECLARE_DELEGATE_OneParam(OnAssetBrowserCreatedSignature, const TSharedRef<SPaperZDAnimAssetBrowser>&);

	/* Called when the animation viewport widget gets created. */
	DECLARE_DELEGATE_OneParam(OnAnimationSourceViewportCreatedSignature, const TSharedRef<SPaperZDAnimationSourceViewport>&);

	/* Called when the details view gets created. */
	DECLARE_DELEGATE_OneParam(OnDetailsViewCreatedSignature, const TSharedRef<IDetailsView>&);

	/* Called when a group of objects is selected. */
	DECLARE_DELEGATE_OneParam(OnObjectsSelectedSignature, const TArray<UObject*>&);

	/* Called to invoke a specified tab. */
	DECLARE_DELEGATE_OneParam(OnInvokeTabSignature, const FTabId&);
}
