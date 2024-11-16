// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/PaperZDNotifyGraphSchema.h"
#include "Graphs/PaperZDNotifyGraph.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDNotifyGraphSchema)
#endif

#define LOCTEXT_NAMESPACE "ZDNotifyGraph"

UPaperZDNotifyGraphSchema::UPaperZDNotifyGraphSchema(const FObjectInitializer& ObjectInitializer)
	: Super()
{
}

void UPaperZDNotifyGraphSchema::GetGraphDisplayInformation(const UEdGraph& Graph, /*out*/ FGraphDisplayInfo& DisplayInfo) const
{
	const UPaperZDNotifyGraph* NotifyGraph = CastChecked<UPaperZDNotifyGraph>(&Graph);
	
	FFormatNamedArguments Args;
	Args.Add(TEXT("NotifyName"), FText::FromName(NotifyGraph->NotifyName));
	DisplayInfo.DisplayName = NotifyGraph->bRegistered ? FText::Format(LOCTEXT("ZDNotifyName", "Receive Notify: {NotifyName}"), Args) : FText::Format(LOCTEXT("ZDNotifyName_Unreg", "(Unregistered) Notify: {NotifyName}"), Args);
	DisplayInfo.PlainName = DisplayInfo.DisplayName;
	DisplayInfo.Tooltip = NotifyGraph->bRegistered ? FText::Format(LOCTEXT("ZDNotifyGraphTooltip", "Custom AnimNotify {NotifyName}"), Args) : FText::Format(LOCTEXT("ZDNotifyGraphTooltip_Unreg", "Custom AnimNotify {NotifyName} (Unregistered)"), Args);
}

#undef LOCTEXT_NAMESPACE
