// Copyright 2017 ~ 2023 Critical Failure Studio Ltd. All rights reserved.

#include "Editors/DetailCustomizations/PaperZDComponentReferenceCustomization.h"
#include "PaperZDComponentReference.h"
#include "ActorPickerMode.h"
#include "Brushes/SlateNoResource.h"
#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "Containers/UnrealString.h"
#include "Delegates/Delegate.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Engine/EngineTypes.h"
#include "Engine/LevelScriptActor.h"
#include "Fonts/SlateFontInfo.h"
#include "GameFramework/Actor.h"
#include "HAL/PlatformCrt.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailPropertyRow.h"
#include "Internationalization/Internationalization.h"
#include "Kismet2/ComponentEditorUtils.h"
#include "Layout/BasicLayoutWidgetSlot.h"
#include "Layout/Margin.h"
#include "Math/Color.h"
#include "Misc/AssertionMacros.h"
#include "Misc/Attribute.h"
#include "PropertyCustomizationHelpers.h"
#include "PropertyHandle.h"
#include "SlotBase.h"
#include "Styling/AppStyle.h"
#include "Styling/SlateColor.h"
#include "Styling/SlateIconFinder.h"
#include "Templates/Casts.h"
#include "Types/SlateEnums.h"
#include "UObject/Class.h"
#include "UObject/Field.h"
#include "UObject/GarbageCollection.h"
#include "UObject/NameTypes.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ObjectPtr.h"
#include "UObject/PropertyPortFlags.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/UObjectIterator.h"
#include "UObject/UnrealType.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SNullWidget.h"
#include "Widgets/Text/STextBlock.h"
#include "SubobjectDataSubsystem.h" 	
#include "SubobjectDataHandle.h"

#include "Editors/Slate/SPaperZDComponentSelector.h"

static const FName NAME_AllowedClasses = "AllowedClasses";
static const FName NAME_DisallowedClasses = "DisallowedClasses";
static const FName NAME_UseComponentPicker = "UseComponentPicker";

#define LOCTEXT_NAMESPACE "SelfComponentDetailCustomization"

TSharedRef<IPropertyTypeCustomization> FPaperZDComponentReferenceCustomization::MakeInstance()
{
	return MakeShareable(new FPaperZDComponentReferenceCustomization);
}

void FPaperZDComponentReferenceCustomization::CustomizeHeader(TSharedRef<class IPropertyHandle> InPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& PropertyTypeCustomizationUtils)
{
	PropertyHandle = InPropertyHandle;

	CachedComponent.Reset();
	CachedFirstOuterActor.Reset();
	CachedPropertyAccess = FPropertyAccess::Fail;

	bAllowClear = false;
	bUseComponentPicker = PropertyHandle->HasMetaData(NAME_UseComponentPicker);
	bIsSoftReference = false;

	if (bUseComponentPicker)
	{
		FProperty* Property = InPropertyHandle->GetProperty();
		check(CastField<FStructProperty>(Property) &&
			(FPaperZDComponentReference::StaticStruct() == CastFieldChecked<const FStructProperty>(Property)->Struct ||
				FPaperZDSoftComponentReference::StaticStruct() == CastFieldChecked<const FStructProperty>(Property)->Struct));

		bAllowClear = !(InPropertyHandle->GetMetaDataProperty()->PropertyFlags & CPF_NoClear);
		bIsSoftReference = FSoftComponentReference::StaticStruct() == CastFieldChecked<const FStructProperty>(Property)->Struct;

		BuildClassFilters();
		BuildComboBox();
		
		InPropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FPaperZDComponentReferenceCustomization::OnPropertyValueChanged));

		// set cached values
		{
			CachedComponent.Reset();
			CachedFirstOuterActor = GetFirstOuterActor();

			FComponentReference TmpComponentReference;
			CachedPropertyAccess = GetValue(TmpComponentReference);
			if (CachedPropertyAccess == FPropertyAccess::Success)
			{
				CachedComponent = GetComponentFromReference(TmpComponentReference);
				CachedVariableName = GetCachedComponentDisplayName(TmpComponentReference);
				if (!IsComponentReferenceValid(TmpComponentReference))
				{
					CachedComponent.Reset();
				}
			}
		}

		HeaderRow.NameContent()
		[
			InPropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			ComponentComboButton.ToSharedRef()
		]
		.IsEnabled(MakeAttributeSP(this, &FPaperZDComponentReferenceCustomization::CanEdit));
	}
	else
	{
		HeaderRow.NameContent()
		[
			InPropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			InPropertyHandle->CreatePropertyValueWidget()
		]
		.IsEnabled(MakeAttributeSP(this, &FPaperZDComponentReferenceCustomization::CanEdit));
	}

}

void FPaperZDComponentReferenceCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> InStructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	uint32 NumberOfChild;
	if (InStructPropertyHandle->GetNumChildren(NumberOfChild) == FPropertyAccess::Success)
	{
		for (uint32 Index = 0; Index < NumberOfChild; ++Index)
		{
			TSharedRef<IPropertyHandle> ChildPropertyHandle = InStructPropertyHandle->GetChildHandle(Index).ToSharedRef();
			if (bUseComponentPicker)
			{
				ChildPropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FPaperZDComponentReferenceCustomization::OnPropertyValueChanged));
				StructBuilder.AddProperty(ChildPropertyHandle)
					.ShowPropertyButtons(true)
					.IsEnabled(MakeAttributeSP(this, &FPaperZDComponentReferenceCustomization::CanEditChildren));
			}
			else
			{
				StructBuilder.AddProperty(ChildPropertyHandle)
					.ShowPropertyButtons(true)
					.IsEnabled(MakeAttributeSP(this, &FPaperZDComponentReferenceCustomization::CanEditChildren));
			}
		}
	}
}

void FPaperZDComponentReferenceCustomization::BuildClassFilters()
{
	auto AddToClassFilters = [this](UClass* Class, TArray<TSubclassOf<UActorComponent>>& ComponentList)
	{
		if (Class->IsChildOf(UActorComponent::StaticClass()))
		{
			ComponentList.Add(Class);
		}
	};

	auto ParseClassFilters = [this, AddToClassFilters](const FString& MetaDataString, TArray<TSubclassOf<UActorComponent>>& ComponentList)
	{
		if (!MetaDataString.IsEmpty())
		{
			TArray<FString> ClassFilterNames;
			MetaDataString.ParseIntoArrayWS(ClassFilterNames, TEXT(","), true);

			for (const FString& ClassName : ClassFilterNames)
			{
				UClass* Class = UClass::TryFindTypeSlow<UClass>(ClassName);
				if (!Class)
				{
					Class = LoadObject<UClass>(nullptr, *ClassName);
				}

				if (Class)
				{
					// If the class is an interface, expand it to be all classes in memory that implement the class.
					if (Class->HasAnyClassFlags(CLASS_Interface))
					{
						for (TObjectIterator<UClass> ClassIt; ClassIt; ++ClassIt)
						{
							UClass* ClassWithInterface = (*ClassIt);
							if (ClassWithInterface->ImplementsInterface(Class))
							{
								AddToClassFilters(ClassWithInterface, ComponentList);
							}
						}
					}
					else
					{
						AddToClassFilters(Class, ComponentList);
					}
				}
			}
		}
	};

	// Account for the allowed classes specified in the property metadata
	const FString& AllowedClassesFilterString = PropertyHandle->GetMetaData(NAME_AllowedClasses);
	ParseClassFilters(AllowedClassesFilterString, AllowedComponentClassFilters);

	const FString& DisallowedClassesFilterString = PropertyHandle->GetMetaData(NAME_DisallowedClasses);
	ParseClassFilters(DisallowedClassesFilterString, DisallowedComponentClassFilters);
}

void FPaperZDComponentReferenceCustomization::BuildComboBox()
{
	TAttribute<bool> IsEnabledAttribute(this, &FPaperZDComponentReferenceCustomization::CanEdit);
	TAttribute<FText> TooltipAttribute;
	if (PropertyHandle->GetMetaDataProperty()->HasAnyPropertyFlags(CPF_EditConst | CPF_DisableEditOnTemplate))
	{
		TArray<UObject*> ObjectList;
		PropertyHandle->GetOuterObjects(ObjectList);

		// If there is no objects, that means we must have a struct asset managing this property
		if (ObjectList.Num() == 0)
		{
			IsEnabledAttribute.Set(false);
			TooltipAttribute.Set(LOCTEXT("VariableHasDisableEditOnTemplate", "Editing this value in structure's defaults is not allowed"));
		}
		else
		{
			// Go through all the found objects and see if any are a CDO, we can't set an actor in a CDO default.
			for (UObject* Obj : ObjectList)
			{
				if (Obj->IsTemplate() && !Obj->IsA<ALevelScriptActor>())
				{
					IsEnabledAttribute.Set(false);
					TooltipAttribute.Set(LOCTEXT("VariableHasDisableEditOnTemplateTooltip", "Editing this value in a Class Default Object is not allowed"));
					break;
				}

			}
		}
	}

	TSharedRef<SVerticalBox> ObjectContent = SNew(SVerticalBox);
	ObjectContent->AddSlot()
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		[
			SNew(SImage)
			.Image(this, &FPaperZDComponentReferenceCustomization::GetComponentIcon)
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1)
		.VAlign(VAlign_Center)
		[
			// Show the name of the asset or actor
			SNew(STextBlock)
			.Font(IDetailLayoutBuilder::GetDetailFont())
			.Text(this, &FPaperZDComponentReferenceCustomization::OnGetComponentName)
		]
	];

	TSharedRef<SHorizontalBox> ComboButtonContent = SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		[
			SNew(SImage)
			.Image(this, &FPaperZDComponentReferenceCustomization::GetStatusIcon)
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1)
		.VAlign(VAlign_Center)
		[
			ObjectContent
		];

	ComponentComboButton = SNew(SComboButton)
		.ToolTipText(TooltipAttribute)
		.ButtonStyle(FAppStyle::Get(), "PropertyEditor.AssetComboStyle")
		.ForegroundColor(FAppStyle::GetColor("PropertyEditor.AssetName.ColorAndOpacity"))
		.OnGetMenuContent(this, &FPaperZDComponentReferenceCustomization::OnGetMenuContent)
		.OnMenuOpenChanged(this, &FPaperZDComponentReferenceCustomization::OnMenuOpenChanged)
		.IsEnabled(IsEnabledAttribute)
		.ContentPadding(2.0f)
		.ButtonContent()
		[
			SNew(SWidgetSwitcher)
			.WidgetIndex(this, &FPaperZDComponentReferenceCustomization::OnGetComboContentWidgetIndex)
			+ SWidgetSwitcher::Slot()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("MultipleValuesText", "<multiple values>"))
				.Font(IDetailLayoutBuilder::GetDetailFont())
			]
			+ SWidgetSwitcher::Slot()
			[
				ComboButtonContent
			]
		];
}

AActor* FPaperZDComponentReferenceCustomization::GetFirstOuterActor() const
{
	TArray<UObject*> ObjectList;
	PropertyHandle->GetOuterObjects(ObjectList);
	for (UObject* Obj : ObjectList)
	{
		while (Obj)
		{
			if (AActor* Actor = Cast<AActor>(Obj))
			{
				return Actor;
			}
			else if (UActorComponent* Component = Cast<UActorComponent>(Obj))
			{
				if (Component->GetOwner())
				{
					return Component->GetOwner();
				}
			}
			else if (UClass* ContainerClass = Cast<UClass>(Obj))
			{
				//This use case happens when the property handle belongs to a component we added on the blueprint editor (not through cpp) and we're in the blueprint editor (editing the CDO).
				//In this case the parent of the component is not really the actor, but the blueprint object itself.
				if (AActor* ContainerActor = ContainerClass->GetDefaultObject<AActor>())
				{
					return ContainerActor;
				}
			}

			Obj = Obj->GetOuter();
		}
	}
	UObject * B = PropertyHandle->GetProperty()->GetOutermost();

	return nullptr;
}

void FPaperZDComponentReferenceCustomization::SetValue(const FComponentReference& Value)
{
	ComponentComboButton->SetIsOpen(false);

	const bool bIsEmpty = Value == FComponentReference();
	const bool bAllowedToSetBasedOnFilter = IsComponentReferenceValid(Value);
	if (bIsEmpty || bAllowedToSetBasedOnFilter)
	{
		FString TextValue;
		if (bIsSoftReference)
		{
			FSoftComponentReference SoftValue;
			if (Value.OtherActor.IsValid())
			{
				SoftValue.OtherActor = Value.OtherActor.Get();
				SoftValue.ComponentProperty = Value.ComponentProperty;
				SoftValue.PathToComponent = Value.PathToComponent;
			}
			CastFieldChecked<const FStructProperty>(PropertyHandle->GetProperty())->Struct->ExportText(TextValue, &SoftValue, &SoftValue, nullptr, EPropertyPortFlags::PPF_None, nullptr);
			ensure(PropertyHandle->SetValueFromFormattedString(TextValue) == FPropertyAccess::Result::Success);
		}
		else
		{
			CastFieldChecked<const FStructProperty>(PropertyHandle->GetProperty())->Struct->ExportText(TextValue, &Value, &Value, nullptr, EPropertyPortFlags::PPF_None, nullptr);
			ensure(PropertyHandle->SetValueFromFormattedString(TextValue) == FPropertyAccess::Result::Success);
		}
	}
}

FPropertyAccess::Result FPaperZDComponentReferenceCustomization::GetValue(FComponentReference& OutValue) const
{
	// Potentially accessing the value while garbage collecting or saving the package could trigger a crash.
	// so we fail to get the value when that is occurring.
	if (GIsSavingPackage || IsGarbageCollecting())
	{
		return FPropertyAccess::Fail;
	}

	FPropertyAccess::Result Result = FPropertyAccess::Fail;
	if (PropertyHandle.IsValid() && PropertyHandle->IsValidHandle())
	{
		TArray<void*> RawData;
		PropertyHandle->AccessRawData(RawData);
		UActorComponent const* CurrentComponent = nullptr;
		for (const void* RawPtr : RawData)
		{
			if (RawPtr)
			{
				FComponentReference ThisReference;
				if (bIsSoftReference)
				{
					FSoftComponentReference SoftReference = *reinterpret_cast<const FSoftComponentReference*>(RawPtr);
					if (SoftReference.OtherActor.IsValid())
					{
						ThisReference.OtherActor = SoftReference.OtherActor.Get();
						ThisReference.ComponentProperty = SoftReference.ComponentProperty;
						ThisReference.PathToComponent = SoftReference.PathToComponent;
					}
				}
				else
				{
					ThisReference = *reinterpret_cast<const FComponentReference*>(RawPtr);
				}
				if (Result == FPropertyAccess::Success)
				{
					if (GetComponentFromReference(ThisReference) != CurrentComponent)
					{
						Result = FPropertyAccess::MultipleValues;
						break;
					}
				}
				else
				{
					OutValue = ThisReference;
					CurrentComponent = GetComponentFromReference(OutValue);
					Result = FPropertyAccess::Success;
				}
			}
			else if (Result == FPropertyAccess::Success)
			{
				Result = FPropertyAccess::MultipleValues;
				break;
			}
		}
	}
	return Result;
}

bool FPaperZDComponentReferenceCustomization::IsComponentReferenceValid(const FComponentReference& Value) const
{
	if (Value.OtherActor.IsValid())
	{
		return false;
	}

	if (const UActorComponent* NewComponent = GetComponentFromReference(Value))
	{
		if (!IsFilteredComponent(NewComponent))
		{
			return false;
		}
	}

	return true;
}

void FPaperZDComponentReferenceCustomization::OnPropertyValueChanged()
{
	CachedComponent.Reset();
	CachedFirstOuterActor = GetFirstOuterActor();

	FComponentReference TmpComponentReference;
	CachedPropertyAccess = GetValue(TmpComponentReference);
	if (CachedPropertyAccess == FPropertyAccess::Success)
	{
		CachedComponent = GetComponentFromReference(TmpComponentReference);
		CachedVariableName = GetCachedComponentDisplayName(TmpComponentReference);
		if (!IsComponentReferenceValid(TmpComponentReference))
		{
			CachedComponent.Reset();
			if (!(TmpComponentReference == FComponentReference()))
			{
				SetValue(FComponentReference());
			}
		}
	}
}

int32 FPaperZDComponentReferenceCustomization::OnGetComboContentWidgetIndex() const
{
	switch (CachedPropertyAccess)
	{
	case FPropertyAccess::MultipleValues: return 0;
	case FPropertyAccess::Success:
	default:
		return 1;
	}
}

bool FPaperZDComponentReferenceCustomization::CanEdit() const
{
	return PropertyHandle.IsValid() ? !PropertyHandle->IsEditConst() : true;
}

bool FPaperZDComponentReferenceCustomization::CanEditChildren() const
{
	if (const UActorComponent* Component = CachedComponent.Get())
	{
		if (AActor* Owner = Component->GetOwner())
		{
			return FSlateIconFinder::FindIconBrushForClass(Owner->GetClass()) != nullptr;
		}
	}
	return FSlateIconFinder::FindIconBrushForClass(AActor::StaticClass()) != nullptr;
}

const FSlateBrush* FPaperZDComponentReferenceCustomization::GetComponentIcon() const
{
	if (const UActorComponent* ActorComponent = CachedComponent.Get())
	{
		return FSlateIconFinder::FindIconBrushForClass(ActorComponent->GetClass());
	}
	return FSlateIconFinder::FindIconBrushForClass(UActorComponent::StaticClass());
}

FText FPaperZDComponentReferenceCustomization::OnGetComponentName() const
{
	if (CachedPropertyAccess == FPropertyAccess::Success)
	{
		if (const UActorComponent* ActorComponent = CachedComponent.Get())
		{
			const AActor* OwnerActor = CachedFirstOuterActor.Get();
			const bool bIsArrayVariable = !CachedVariableName.IsNone() && OwnerActor != nullptr && FindFProperty<FArrayProperty>(OwnerActor->GetClass(), CachedVariableName);
			if (!CachedVariableName.IsNone() && !bIsArrayVariable)
			{
				return FText::FromName(CachedVariableName);
			}
			return FText::AsCultureInvariant(ActorComponent->GetName());
		}
	}
	else if (CachedPropertyAccess == FPropertyAccess::MultipleValues)
	{
		return LOCTEXT("MultipleValues", "Multiple Values");
	}
	return LOCTEXT("NoComponent", "None");
}

const FSlateBrush* FPaperZDComponentReferenceCustomization::GetStatusIcon() const
{
	static FSlateNoResource EmptyBrush = FSlateNoResource();

	if (CachedPropertyAccess == FPropertyAccess::Fail)
	{
		return FAppStyle::GetBrush("Icons.Error");
	}
	return &EmptyBrush;
}

TSharedRef<SWidget> FPaperZDComponentReferenceCustomization::OnGetMenuContent()
{
	//return SNullWidget::NullWidget;
	return SNew(SPaperZDComponentSelector, CachedFirstOuterActor.Get())
		.OnComponentSelected(this, &FPaperZDComponentReferenceCustomization::OnComponentSelected)
		.OnClearedSelection(this, &FPaperZDComponentReferenceCustomization::OnClearSelection)
		.AllowClear(bAllowClear)
		.AllowedClassesFilter(AllowedComponentClassFilters)
		.DisallowedClassesFilter(DisallowedComponentClassFilters);
}

void FPaperZDComponentReferenceCustomization::OnMenuOpenChanged(bool bOpen)
{
	if (!bOpen)
	{
		ComponentComboButton->SetMenuContent(SNullWidget::NullWidget);
	}
}

bool FPaperZDComponentReferenceCustomization::IsFilteredComponent(const UActorComponent* const Component) const
{
	return IsFilteredObject(Component, AllowedComponentClassFilters, DisallowedComponentClassFilters);
}

bool FPaperZDComponentReferenceCustomization::IsFilteredObject(const UObject* const Object, const TArray<TSubclassOf<UActorComponent>>& AllowedFilters, const TArray<TSubclassOf<UActorComponent>>& DisallowedFilters)
{
	bool bAllowedToSetBasedOnFilter = true;

	const UClass* ObjectClass = Object->GetClass();
	if (AllowedFilters.Num() > 0)
	{
		bAllowedToSetBasedOnFilter = false;
		for (const UClass* AllowedClass : AllowedFilters)
		{
			const bool bAllowedClassIsInterface = AllowedClass->HasAnyClassFlags(CLASS_Interface);
			if (ObjectClass->IsChildOf(AllowedClass) || (bAllowedClassIsInterface && ObjectClass->ImplementsInterface(AllowedClass)))
			{
				bAllowedToSetBasedOnFilter = true;
				break;
			}
		}
	}

	if (DisallowedFilters.Num() > 0 && bAllowedToSetBasedOnFilter)
	{
		for (const UClass* DisallowedClass : DisallowedFilters)
		{
			const bool bDisallowedClassIsInterface = DisallowedClass->HasAnyClassFlags(CLASS_Interface);
			if (ObjectClass->IsChildOf(DisallowedClass) || (bDisallowedClassIsInterface && ObjectClass->ImplementsInterface(DisallowedClass)))
			{
				bAllowedToSetBasedOnFilter = false;
				break;
			}
		}
	}

	return bAllowedToSetBasedOnFilter;
}

void FPaperZDComponentReferenceCustomization::OnComponentSelected(const UActorComponent* InComponent, const FName VariableName)
{
	ComponentComboButton->SetIsOpen(false);

	FComponentReference ComponentReference = FComponentEditorUtils::MakeComponentReference(CachedFirstOuterActor.Get(), InComponent);
	ComponentReference.ComponentProperty = VariableName;
	SetValue(ComponentReference);
}

void FPaperZDComponentReferenceCustomization::OnClearSelection()
{
	ComponentComboButton->SetIsOpen(false);

	//Clear selection
	CachedComponent.Reset();
	SetValue(FComponentReference());
}

void FPaperZDComponentReferenceCustomization::CloseComboButton()
{
	ComponentComboButton->SetIsOpen(false);
}

const UActorComponent* FPaperZDComponentReferenceCustomization::GetComponentFromReference(const FComponentReference& CompRef) const
{
	//We always prioritize the ComponentReference implementation as that one obtains the 'root' component when no value has been set
	const UActorComponent* OutComponent = CompRef.GetComponent(CachedFirstOuterActor.Get());
	if (!OutComponent)
	{
		//If we failed to obtain the component, it might be because we're on the BlueprintEditor inspecting the CDO which doesn't have the BP components attached yet.
		//We need to use the SubObjects system for this specific case
		USubobjectDataSubsystem* DataSubsystem = USubobjectDataSubsystem::Get();
		check(DataSubsystem);

		//Gather the components and then search for the one with the same variable name
		TArray<FSubobjectDataHandle> SubobjectData;
		DataSubsystem->GatherSubobjectData(CachedFirstOuterActor.Get(), SubobjectData);
		for (const FSubobjectDataHandle& Handle : SubobjectData)
		{
			//This clause here tries to catch as many edge-cases as possible. Comparing variable names against the property name is the easiest way to get the correct component.
			//The issue stems from the 'ComponentProperty' not being filled up normally through code (as the variable names aren't easily grabbable automatically).
			//Manually the users will probably write the component ref name by hand, but in cases in which they use 'InitComponentReference' for example, the PathToComponent will be the only one setup.
			//For BP only variables, this is exactly the same as the component property name though, hence why we compare both.
			TSharedPtr<FSubobjectData> ObjectData = Handle.GetSharedDataPtr();
			const FName VarName = ObjectData->GetVariableName();
			if (!VarName.IsNone() && (VarName == CompRef.ComponentProperty || VarName == *CompRef.PathToComponent))
			{
				//Found it, can assign and break
				OutComponent = ObjectData->GetObject<UActorComponent>();
				break;
			}
		}
	}

	return OutComponent;
}

FName FPaperZDComponentReferenceCustomization::GetCachedComponentDisplayName(const FComponentReference& CompRef)
{
	if (CachedComponent.IsValid())
	{
		if (CompRef == FComponentReference())
		{
			return TEXT("None (Root)");
		}

		//Check if this component has an easy variable we can get 
		const FName TempName = FComponentEditorUtils::FindVariableNameGivenComponentInstance(CachedComponent.Get());
		if (!TempName.IsNone())
		{
			return TempName;
		}
		else if (!CompRef.ComponentProperty.IsNone())
		{
			//No other way to bind for BP created objects inside BP editor.
			return CompRef.ComponentProperty;
		}
		else
		{
			//Only specific if the property was setup using only the path
			return *CompRef.PathToComponent;
		}
	}

	return NAME_None;
}

#undef LOCTEXT_NAMESPACE
