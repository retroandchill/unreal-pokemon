// "Unreal Pokémon" created by Retro & Chill.


#include "Graphics/AssetClasses.h"

namespace Pokemon::Assets::Graphics {

    UE_DEFINE_ASSET_CLASS(PokemonSprites, UPaperFlipbook, "/Game/Graphics/Pokemon", "");
    
    UE_DEFINE_ASSET_CLASS(TrainerSprites, UPaperFlipbook, "/Game/Graphics/Trainers", "");
    
    UE_DEFINE_ASSET_CLASS(TypeIcons, UObject, "/Game/Graphics/UI/Types", "types_");
    UE_DEFINE_ASSET_CLASS(TypePanels, UObject, "/Game/Graphics/UI/Battle/Moves", "move_");
    UE_DEFINE_ASSET_CLASS(StatusIcons, UObject, "/Game/Graphics/UI/Statuses", "status_");
    UE_DEFINE_ASSET_CLASS(SummaryBalls, UObject, "/Game/Graphics/UI/Summary/BallIcons", "icon_ball_");
    UE_DEFINE_ASSET_CLASS(ItemIcons, UObject, "/Game/Graphics/Items", "");
    
}