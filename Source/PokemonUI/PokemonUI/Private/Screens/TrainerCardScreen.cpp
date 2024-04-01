// "Unreal Pokémon" created by Retro & Chill.


#include "Screens/TrainerCardScreen.h"

#include "Asserts.h"
#include "Components/Image.h"
#include "Managers/PokemonSubsystem.h"
#include "Primatives/DisplayText.h"
#include "Utilities/GraphicsLoadingSubsystem.h"

void UTrainerCardScreen::NativeConstruct() {
	Super::NativeConstruct();

	auto PokemonSubsystem = GetGameInstance()->GetSubsystem<UPokemonSubsystem>();
	ASSERT(PokemonSubsystem != nullptr);
	Trainer = PokemonSubsystem->GetPlayer();

	SetTrainerSprite();
}

void UTrainerCardScreen::SetTrainerSprite() {
	ASSERT(TrainerImage != nullptr)

	auto GraphicsLoadingSubsystem = GetGameInstance()->GetSubsystem<UGraphicsLoadingSubsystem>();
	ASSERT(GraphicsLoadingSubsystem != nullptr)
	auto [Material, Size] = GraphicsLoadingSubsystem->GetTrainerSprite(*Trainer, this);
	TrainerImage->SetBrushFromMaterial(Material);
	TrainerImage->SetDesiredSizeOverride(Size);
}

void UTrainerCardScreen::SetTrainerInfo() {
	ASSERT(TrainerName != nullptr)
	TrainerName->SetText(Trainer->GetName());
}