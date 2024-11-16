// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Editors/PaperZDAnimationSourceEditor.h"
#include "Editors/Util/PaperZDEditorDelegates.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"
#include "WorkflowOrientedApp/WorkflowUObjectDocuments.h"

/* Source details. */
class FPaperZDSourceDetailsTabSummoner : public FWorkflowTabFactory
{
	/* Hosting app pointer. */
	TWeakPtr<FPaperZDAnimationSourceEditor> HostingApp;

public:
	//ctor
	FPaperZDSourceDetailsTabSummoner(TSharedPtr<FPaperZDAnimationSourceEditor> InHostingApp);

	//FWorkflowTabFactory
	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;
	//End FWorkflowTabFactory
};

/* Asset browser. */
class FPaperZDAssetBrowserTabSummoner : public FWorkflowTabFactory
{
	/* Hosting app pointer. */
	TWeakPtr<FPaperZDAnimationSourceEditor> HostingApp;

	/* Delegate for the open asset functionality. */
	FPaperZDEditorDelegates::OnOpenAssetSignature OnOpenAsset;

	/* Delegate for the asset browser created call. */
	FPaperZDEditorDelegates::OnAssetBrowserCreatedSignature OnAssetBrowserCreated;

public:
	//ctor
	FPaperZDAssetBrowserTabSummoner(TSharedPtr<FPaperZDAnimationSourceEditor> InHostingApp, FPaperZDEditorDelegates::OnOpenAssetSignature InOnOpenAsset, FPaperZDEditorDelegates::OnAssetBrowserCreatedSignature InOnAssetBrowserCreated);

	//FWorkflowTabFactory
	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;
	//End FWorkflowTabFactory
};

/* Asset details. */
class FPaperZDAssetDetailsTabSummoner : public FWorkflowTabFactory
{
	/* Hosting app pointer. */
	TWeakPtr<FPaperZDAnimationSourceEditor> HostingApp;

	/* Delegate for the asset browser created call. */
	FPaperZDEditorDelegates::OnGetAssetSignature OnGetAsset;

public:
	//ctor
	FPaperZDAssetDetailsTabSummoner(TSharedPtr<FPaperZDAnimationSourceEditor> InHostingApp, FPaperZDEditorDelegates::OnGetAssetSignature InOnGetAsset);

	//FWorkflowTabFactory
	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;
	//End FWorkflowTabFactory
};

/* Viewport tab. */
class FPaperZDAnimationViewportTabSummoner : public FWorkflowTabFactory
{
	/* Hosting app pointer. */
	TWeakPtr<FPaperZDAnimationSourceEditor> HostingApp;

	/* Delegate for the viewport created call. */
	FPaperZDEditorDelegates::OnAnimationSourceViewportCreatedSignature OnViewportCreated;

public:
	//ctor
	FPaperZDAnimationViewportTabSummoner(TSharedPtr<FPaperZDAnimationSourceEditor> InHostingApp, FPaperZDEditorDelegates::OnAnimationSourceViewportCreatedSignature InOnViewportCreated);

	//FWorkflowTabFactory
	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;
	//End FWorkflowTabFactory
};

/* Details view tab. */
class FPaperZDDetailsViewTabSummoner : public FWorkflowTabFactory
{
	/* Hosting app pointer. */
	TWeakPtr<FPaperZDAnimationSourceEditor> HostingApp;

	/* Delegate for the viewport created call. */
	FPaperZDEditorDelegates::OnDetailsViewCreatedSignature OnDetailsViewCreated;

public:
	//ctor
	FPaperZDDetailsViewTabSummoner(TSharedPtr<FPaperZDAnimationSourceEditor> InHostingApp, FPaperZDEditorDelegates::OnDetailsViewCreatedSignature OnDetailsViewCreated);

	//FWorkflowTabFactory
	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;
	//End FWorkflowTabFactory
};

/* Manage notifies tab. */
class FPaperZDSourceNotifiesTabSummoner : public FWorkflowTabFactory
{
	/* Hosting app pointer. */
	TWeakPtr<FPaperZDAnimationSourceEditor> HostingApp;

public:
	//ctor
	FPaperZDSourceNotifiesTabSummoner(TSharedPtr<FPaperZDAnimationSourceEditor> InHostingApp);

	//FWorkflowTabFactory
	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;
	//End FWorkflowTabFactory
};
