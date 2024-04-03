// "Unreal Pokémon" created by Retro & Chill.


#include "Pokemon/PokemonBuilder.h"

#include "PokemonCoreSettings.h"
#include "Pokemon/GamePokemon.h"
#include "Pokemon/Pokemon.h"
#include "UObject/Initializers.h"

UPokemonBuilder* UPokemonBuilder::Species(FName Species) {
	DTO.Species = Species;
	return this;
}

UPokemonBuilder* UPokemonBuilder::Level(int32 Level) {
	DTO.StatBlock.Level = Level;
	return this;
}

UPokemonBuilder* UPokemonBuilder::PersonalityValue(uint32 PersonalityValue) {
	DTO.PersonalityValue = PersonalityValue;
	return this;
}

UPokemonBuilder* UPokemonBuilder::Nickname(FText Nickname) {
	DTO.Nickname = Nickname;
	return this;
}

UPokemonBuilder* UPokemonBuilder::Gender(EPokemonGender Gender) {
	DTO.Gender = Gender;
	return this;
}

UPokemonBuilder* UPokemonBuilder::Shiny(bool bShiny) {
	DTO.bShiny = bShiny;
	return this;
}

UPokemonBuilder* UPokemonBuilder::StatBlock(const FStatBlockDTO& StatBlock) {
	DTO.StatBlock = StatBlock;
	return this;
}

UPokemonBuilder* UPokemonBuilder::StatBlock(FStatBlockDTO&& StatBlock) {
	DTO.StatBlock = MoveTemp(StatBlock);
	return this;
}

UPokemonBuilder* UPokemonBuilder::Exp(int32 Exp) {
	DTO.StatBlock.Exp = Exp;
	return this;
}

UPokemonBuilder* UPokemonBuilder::CurrentHP(int32 CurrentHP) {
	DTO.CurrentHP = CurrentHP;
	return this;
}

UPokemonBuilder* UPokemonBuilder::IVs(const TMap<FName, int32>& IVs) {
	DTO.StatBlock.IVs = IVs;
	return this;
}

UPokemonBuilder* UPokemonBuilder::IVs(TMap<FName, int32>&& IVs) {
	DTO.StatBlock.IVs = MoveTemp(IVs);
	return this;
}

UPokemonBuilder* UPokemonBuilder::IV(FName Stat, int32 IV) {
	DTO.StatBlock.IVs.Add(Stat, IV);
	return this;
}

UPokemonBuilder* UPokemonBuilder::EVs(const TMap<FName, int32>& EVs) {
	DTO.StatBlock.EVs = EVs;
	return this;
}

UPokemonBuilder* UPokemonBuilder::EVs(TMap<FName, int32>&& EVs) {
	DTO.StatBlock.EVs = MoveTemp(EVs);
	return this;
}

UPokemonBuilder* UPokemonBuilder::EV(FName Stat, int32 EV) {
	DTO.StatBlock.EVs.Add(Stat, EV);
	return this;
}

UPokemonBuilder* UPokemonBuilder::Nature(FName Nature) {
	DTO.StatBlock.Nature = Nature;
	return this;
}

UPokemonBuilder* UPokemonBuilder::Ability(int32 AbilityIndex) {
	// TODO: Implement me
	return this;
}

UPokemonBuilder* UPokemonBuilder::Moves(const TArray<FName>& Moves) {
	// TODO: Implement me
	return this;
}

UPokemonBuilder* UPokemonBuilder::Moves(TArray<FName>&& Moves) {
	// TODO: Implement me
	return this;
}

UPokemonBuilder* UPokemonBuilder::Item(FName Item) {
	// TODO: Implement me
	return this;
}

const FPokemonDTO& UPokemonBuilder::GetDTO() const {
	return DTO;
}

TScriptInterface<IPokemon> UPokemonBuilder::Build(UObject* Outer) const {
	auto Settings = GetDefault<UPokemonCoreSettings>();
	ASSERT(Settings != nullptr)
	auto PokemonClass = Settings->GetPokemonClass();
		
	TScriptInterface<IPokemon> Ret = NewObject<UObject>(Outer, PokemonClass);
	Ret->Initialize(DTO);
	return Ret;
}
