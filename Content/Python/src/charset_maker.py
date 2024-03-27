import os

from unreal import Texture2D, Array, PaperSprite, PaperFlipbook, AssetToolsHelpers, PaperSpriteFactory, \
    PaperFlipbookFactory, Vector2D, SpritePivotMode, EditorAssetLibrary, PaperFlipbookKeyFrame, Package, Charset, \
    CharsetFactory, DirectionalSprite, Set

TOTAL_FLIPBOOKS = 4

def extract_sprites(source_texture: Texture2D, rows: int, columns: int) -> Array[PaperSprite]:
    """
    Extracts sprites from the given source texture
    :param source_texture: The texture to extract sprites from
    :param rows: The number of rows in the spriteset
    :param columns: The number of columns in the sprite set
    :return: The created PaperSprite assets
    """
    asset_tools = AssetToolsHelpers.get_asset_tools()
    sprite_factory = PaperSpriteFactory()
    texture_size = Vector2D(source_texture.blueprint_get_size_x(), source_texture.blueprint_get_size_y())
    sprite_size = Vector2D(texture_size.x / columns, texture_size.y / rows)

    sprites = Array(PaperSprite)
    name_base = source_texture.get_name()
    path_name = os.path.dirname(source_texture.get_path_name())
    for row in range(rows):
        for col in range(columns):
            index = row * columns + col
            sprite = PaperSprite.cast(asset_tools.create_asset(f'{name_base}_Sprite_{index}', path_name,
                                                               PaperSprite.static_class(), sprite_factory))
            sprite.set_editor_property("source_texture", source_texture)
            sprite.set_editor_property("source_dimension", sprite_size)
            sprite.set_editor_property("source_texture_dimension", texture_size)
            sprite.set_editor_property("pivot_mode", SpritePivotMode.BOTTOM_CENTER)
            sprite.set_editor_property("source_uv", Vector2D(sprite_size.x * col,
                                                             sprite_size.y * row))
            EditorAssetLibrary.save_loaded_asset(sprite, False)

            sprites.append(sprite)

    return sprites


def create_flipbooks(sprites: Array[PaperSprite], base_name: str, frame_rate: float) -> Array[PaperFlipbook]:
    """
    Take a list of sprites and create a set of flipbooks from them
    :param sprites: The PaperSprite assets to process
    :param base_name: The base name for the created flipbooks
    :param frame_rate: The base framerate for the created flipbooks
    :return: The create flipbooks
    """
    asset_tools = AssetToolsHelpers.get_asset_tools()
    flipbook_factory = PaperFlipbookFactory()
    sprites_per_flipbook = int(len(sprites) / TOTAL_FLIPBOOKS)
    flipbooks = Array(PaperFlipbook)
    for i in range(TOTAL_FLIPBOOKS):
        path_name = os.path.dirname(sprites[sprites_per_flipbook * i].get_path_name())
        flipbook = PaperFlipbook.cast(asset_tools.create_asset(f'{base_name}_Flipbook_{i}', path_name,
                                                               PaperFlipbook.static_class(), flipbook_factory))
        flipbook.set_editor_property("frames_per_second", frame_rate)

        animation_frames = []
        for j in range(sprites_per_flipbook):
            frame = PaperFlipbookKeyFrame()
            frame.set_editor_property("sprite", sprites[i * sprites_per_flipbook + j])
            frame.set_editor_property("frame_run", 1)
            animation_frames.append(frame)

        flipbook.set_editor_property("key_frames", animation_frames)
        EditorAssetLibrary.save_loaded_asset(flipbook, False)
        flipbooks.append(flipbook)

    return flipbooks


def convert_flipbooks_to_charset(flipbooks: Array[PaperFlipbook], package_name: str, valid_stop_frames: Set[int]) -> Charset:
    asset_tools = AssetToolsHelpers.get_asset_tools()
    charset_factory = CharsetFactory()
    package_path = os.path.dirname(package_name)
    base_name = os.path.basename(package_name).split('.')[0]
    charset = Charset.cast(asset_tools.create_asset(f'{base_name}_Charset', package_path, Charset.static_class(), charset_factory))

    down_sprite = DirectionalSprite()
    down_sprite.set_editor_property("flipbook", flipbooks[0])
    down_sprite.set_editor_property("valid_stop_frames", valid_stop_frames)
    charset.set_editor_property("down_sprite", down_sprite)

    left_sprite = DirectionalSprite()
    left_sprite.set_editor_property("flipbook", flipbooks[1])
    left_sprite.set_editor_property("valid_stop_frames", valid_stop_frames)
    charset.set_editor_property("left_sprite", left_sprite)

    right_sprite = DirectionalSprite()
    right_sprite.set_editor_property("flipbook", flipbooks[2])
    right_sprite.set_editor_property("valid_stop_frames", valid_stop_frames)
    charset.set_editor_property("right_sprite", right_sprite)

    up_sprite = DirectionalSprite()
    up_sprite.set_editor_property("flipbook", flipbooks[3])
    up_sprite.set_editor_property("valid_stop_frames", valid_stop_frames)
    charset.set_editor_property("up_sprite", up_sprite)

    EditorAssetLibrary.save_loaded_asset(charset, False)
    return charset