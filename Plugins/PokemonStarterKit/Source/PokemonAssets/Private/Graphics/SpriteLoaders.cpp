#include "Graphics/SpriteLoaders.h"
#include "Settings/SettingsStructs.h"

FSpriteLoaders::FSpriteLoaders() = default;

FSpriteLoaders::FSpriteLoaders(const FSpriteRepositories &Settings) :
    PokemonSpriteRepository(Settings.PokemonSpriteRepository),
    TrainerFrontSpriteRepository(Settings.TrainerFrontSpriteRepository),
TypeIconRepository(Settings.TypeIconRepository),
StatusIconRepository(Settings.StatusIconRepository),
SummaryBallRepository(Settings.SummaryBallRepository),
ItemIconRepository(Settings.ItemIconRepository) {
}

FSpriteLoaders & FSpriteLoaders::operator=(const FSpriteRepositories &Settings) {
    PokemonSpriteRepository = Settings.PokemonSpriteRepository;
    TrainerFrontSpriteRepository = Settings.TrainerFrontSpriteRepository;
    TypeIconRepository = Settings.TypeIconRepository;
    StatusIconRepository = Settings.StatusIconRepository;
    SummaryBallRepository = Settings.SummaryBallRepository;
    ItemIconRepository = Settings.ItemIconRepository;
    return *this;
}