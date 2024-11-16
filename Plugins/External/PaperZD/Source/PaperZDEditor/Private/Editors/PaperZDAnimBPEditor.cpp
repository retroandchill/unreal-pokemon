// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Editors/PaperZDAnimBPEditor.h"
#include "Editors/Slate/SPaperZDModeSelectorWidget.h"
#include "PaperZDEditor.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimBP.h"
#include "Toolkits/IToolkit.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/DebuggerCommands.h"
#include "SKismetInspector.h"
#include "ToolMenus.h" 	
#include "EdGraph/EdGraph.h"
#include "Graphs/Nodes/PaperZDAnimGraphNode_StateMachine.h"
#include "Graphs/Nodes/PaperZDStateGraphNode.h"
#include "SBlueprintEditorToolbar.h"
#include "Editor.h"

#define LOCTEXT_NAMESPACE "PaperZDAnimBPEditor"

FPaperZDAnimBPEditor::FPaperZDAnimBPEditor() 
	: AnimBPBeingEdited(nullptr)
{}

FPaperZDAnimBPEditor::~FPaperZDAnimBPEditor()
{}

void FPaperZDAnimBPEditor::DeleteSelectedNodes()
{
	// Make sure that if we're deleting any StateMachine related nodes, we also close the document
	//tab related to it, otherwise we could have a dangling tabs with invalid info inside.
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*NodeIt))
		{
			if (Node->CanUserDeleteNode())
			{
				auto CloseAllDocumentsTab = [this](const UEdGraphNode* InNode)
				{
					TArray<UObject*> NodesToClose;
					GetObjectsWithOuter(InNode, NodesToClose);
					for (UObject* Node : NodesToClose)
					{
						UEdGraph* NodeGraph = Cast<UEdGraph>(Node);
						if (NodeGraph)
						{
							CloseDocumentTab(NodeGraph);
						}
					}
				};


				if (Node->IsA<UPaperZDStateGraphNode>() ||
					Node->IsA<UPaperZDAnimGraphNode_StateMachine>())
				{
					CloseAllDocumentsTab(Node);
				}
			}
		}
	}

	//Call to super to continue
	FBlueprintEditor::DeleteSelectedNodes();
}

FGraphAppearanceInfo FPaperZDAnimBPEditor::GetGraphAppearance(class UEdGraph* InGraph) const
{
	FGraphAppearanceInfo AppearanceInfo = FBlueprintEditor::GetGraphAppearance(InGraph);

	if (GetBlueprintObj()->IsA(UPaperZDAnimBP::StaticClass()))
	{
		AppearanceInfo.CornerText = LOCTEXT("PaperZDAppearanceCornerText_Animation", "2D ANIMATION");
	}

	return AppearanceInfo;
}

//FGCObject Interface
void FPaperZDAnimBPEditor::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(AnimBPBeingEdited);
}
//END FGCObject Interface

//FAssetEditorToolkit
FName FPaperZDAnimBPEditor::GetToolkitFName() const
{
	return FName("PaperZDAnimBPEditor");
}

FText FPaperZDAnimBPEditor::GetBaseToolkitName() const
{
	return LOCTEXT("PaperZDAnimBPEditorAppLabel", "PaperZD AnimBP Editor");
}

FString FPaperZDAnimBPEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("PaperZD AnimBPEditor");
}

FText FPaperZDAnimBPEditor::GetToolkitName() const
{
	return FText::FromString(AnimBPBeingEdited->GetName());
}

FText FPaperZDAnimBPEditor::GetToolkitToolTipText() const
{
	return FAssetEditorToolkit::GetToolTipTextForObject(AnimBPBeingEdited);
}

FLinearColor FPaperZDAnimBPEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor::White;
}
//End FAssetEditorToolkit

void FPaperZDAnimBPEditor::InitAnimBPEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost >& InitToolkitHost, UPaperZDAnimBP* InitAnimBP)
{
	FPaperZDEditorModule& ZDEditorModule = FModuleManager::LoadModuleChecked<FPaperZDEditorModule>("PaperZDEditor");
	AnimBPBeingEdited = InitAnimBP;
	
	if (!Toolbar.IsValid())
	{
		Toolbar = MakeShareable(new FBlueprintEditorToolbar(SharedThis(this)));
	}

	//Don't forget to append the world commands, the play button and others won't do anything if not connected
	GetToolkitCommands()->Append(FPlayWorldCommands::GlobalPlayWorldActions.ToSharedRef());

	//This was originally elsewhere, without this PZD crashes if a PZD blueprint gets opened first before a normal blueprint
	CreateDefaultCommands();

	//Support Undo
	GEditor->RegisterForUndo(this);

	const TSharedRef<FTabManager::FLayout> DummyLayout = FTabManager::NewLayout("NullLayout")->AddArea(FTabManager::NewPrimaryArea());
	
	//Init the asset editor window
	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, TEXT("PaperZDAnimBPEditorApp"), DummyLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, InitAnimBP, false);

	//Common initialize the blueprint
	TArray<UBlueprint*> AnimBlueprints;
	AnimBlueprints.Add(InitAnimBP);

	//Must do
	CommonInitialization(AnimBlueprints, false);

	//Extensions ~ Toolbar
	ExtendToolbar();
	RegenerateMenusAndToolbars();

	//Extensions ~ Detail views
	ZDEditorModule.CustomizeAnimBlueprintEditorDetails(Inspector->GetPropertyView().ToSharedRef());
	
	//Make sure the BlueprintMode register itself
	RegisterApplicationModes(AnimBlueprints, false);

	//Let know that we initialized the view
	PostLayoutBlueprintEditorInitialization();		
}

void FPaperZDAnimBPEditor::ExtendToolbar()
{
	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension(
		"Debugging",
		EExtensionHook::After,
		NULL,
		FToolBarExtensionDelegate::CreateLambda([this](FToolBarBuilder& ToolbarBuilder) 
		{
			AddToolbarWidget(SNew(SPaperZDModeSelectorWidget, AnimBPBeingEdited->GetSupportedAnimationSource())
				.DefaultAnimBP(AnimBPBeingEdited)
				.bSourceSelected(false));
		}));

	//Add the extender
	AddToolbarExtender(ToolbarExtender);
}

void FPaperZDAnimBPEditor::RegisterDefaultEventNodes()
{
	//For now we register using the static class, as there is only one default node
	FKismetEditorUtilities::RegisterAutoGeneratedDefaultEvent(UPaperZDAnimInstance::StaticClass(), UPaperZDAnimInstance::StaticClass(), FName(TEXT("OnTick")));
	FKismetEditorUtilities::RegisterAutoGeneratedDefaultEvent(UPaperZDAnimInstance::StaticClass(), UPaperZDAnimInstance::StaticClass(), FName(TEXT("OnInit")));
}

void FPaperZDAnimBPEditor::UnregisterDefaultEventNodes()
{
	FKismetEditorUtilities::UnregisterAutoBlueprintNodeCreation(UPaperZDAnimInstance::StaticClass());
}
#undef LOCTEXT_NAMESPACE
