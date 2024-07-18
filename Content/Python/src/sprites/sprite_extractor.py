from unreal import Texture2D, PaperSprite, PaperSpriteFactory, AssetToolsHelpers, Vector2D, SpritePivotMode

DIRECTIONS = [2, 4, 6, 8]


def create_sprites_from_texture(source_texture: Texture2D, columns: int, grid_size: int) -> list[PaperSprite]:
    asset_tools = AssetToolsHelpers.get_asset_tools()
    factory = PaperSpriteFactory()
    texture_package = (source_texture.get_path_name()
                       .removesuffix(f'{source_texture.get_name()}.{source_texture.get_name()}'))

    cell_size = Vector2D(source_texture.blueprint_get_size_x() / columns,
                         source_texture.blueprint_get_size_y() / len(DIRECTIONS))
    sprites = []
    for i, direction in enumerate(DIRECTIONS):
        for j in range(columns):
            new_sprite = asset_tools.create_asset(f'{source_texture.get_name()}_Sprite_{len(sprites)}',
                                                  texture_package, PaperSprite.static_class(), factory)
            if not isinstance(new_sprite, PaperSprite):
                raise RuntimeError('Invalid asset type created')

            offset = Vector2D(i * cell_size.x, j * cell_size.y)

            new_sprite.set_editor_property('source_texture', source_texture)
            new_sprite.set_editor_property('source_uv', offset)
            new_sprite.set_editor_property('source_dimension', cell_size)
            new_sprite.set_editor_property('pivot_mode', SpritePivotMode.CUSTOM)
            new_sprite.set_editor_property('custom_pivot_point', Vector2D(offset.x + cell_size.x / 2,
                                                                          offset.y + cell_size.y - grid_size / 2))
            sprites.append(new_sprite)

    return sprites

