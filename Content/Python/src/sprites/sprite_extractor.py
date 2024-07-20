from unreal import Texture2D, PaperSprite, PaperSpriteFactory, AssetToolsHelpers, Vector2D, SpritePivotMode, \
    PaperFlipbook, PaperFlipbookFactory, Array, PaperFlipbookKeyFrame, PaperZDAnimationSource, PaperZDAnimSequence, \
    Object, PaperZDAnimSequenceFactory, PaperZDAnimSequence_Flipbook, PaperZDEditorHelpers, Name

INVALID_ASSET_ERROR = 'Invalid asset type created'

DIRECTIONS = [2, 4, 6, 8]


def get_package_name(texture: Object) -> str:
    return texture.get_path_name().removesuffix(f'{texture.get_name()}.{texture.get_name()}')


def create_sprites_from_texture(source_texture: Texture2D, columns: int, grid_size: int) -> list[PaperSprite]:
    asset_tools = AssetToolsHelpers.get_asset_tools()
    factory = PaperSpriteFactory()
    texture_package = get_package_name(source_texture)

    cell_size = Vector2D(source_texture.blueprint_get_size_x() / columns,
                         source_texture.blueprint_get_size_y() / len(DIRECTIONS))
    sprites = []
    for i, direction in enumerate(DIRECTIONS):
        for j in range(columns):
            new_sprite = asset_tools.create_asset(f'{source_texture.get_name()}_Sprite_{len(sprites)}',
                                                  texture_package, PaperSprite.static_class(), factory)
            if not isinstance(new_sprite, PaperSprite):
                raise RuntimeError(INVALID_ASSET_ERROR)

            offset = Vector2D(j * cell_size.x, i * cell_size.y)

            new_sprite.set_editor_property('source_texture', source_texture)
            new_sprite.set_editor_property('source_uv', offset)
            new_sprite.set_editor_property('source_dimension', cell_size)
            new_sprite.set_editor_property('pivot_mode', SpritePivotMode.CUSTOM)
            new_sprite.set_editor_property('custom_pivot_point', Vector2D(offset.x + cell_size.x / 2,
                                                                          offset.y + cell_size.y - grid_size / 2))
            sprites.append(new_sprite)

    return sprites


def compile_sprites_into_flipbooks(source_texture: Texture2D, sprites: list[PaperSprite], columns: int,
                                    frame_rate: float) -> list[PaperFlipbook]:
    asset_tools = AssetToolsHelpers.get_asset_tools()
    factory = PaperFlipbookFactory()
    texture_package = get_package_name(source_texture)

    flipbooks = []
    for i, direction in enumerate(DIRECTIONS):
        new_flipbook = asset_tools.create_asset(f'{source_texture.get_name()}_D{direction}', texture_package,
                                                PaperFlipbook.static_class(), factory)
        if not isinstance(new_flipbook, PaperFlipbook):
            raise RuntimeError(INVALID_ASSET_ERROR)

        key_frames: Array[PaperFlipbookKeyFrame] = Array(PaperFlipbookKeyFrame)
        for j in range(columns * i, columns * (i + 1)):
            frame = PaperFlipbookKeyFrame()
            frame.set_editor_property('sprite', sprites[j])
            key_frames.append(frame)

        new_flipbook.set_editor_property('key_frames', key_frames)
        new_flipbook.set_editor_property('frames_per_second', frame_rate)
        flipbooks.append(new_flipbook)

    return flipbooks


def place_flipbooks_in_animation_source(source_texture: Texture2D, flipbooks: list[PaperFlipbook],
                                        animation_source: PaperZDAnimationSource) -> PaperZDAnimSequence:
    asset_tools = AssetToolsHelpers.get_asset_tools()
    factory = PaperZDAnimSequenceFactory()
    sequence_package = f'{get_package_name(animation_source)}/AnimSequences'
    factory.set_editor_property('supported_class', PaperZDAnimSequence_Flipbook)
    PaperZDEditorHelpers.set_target_anim_source(factory, animation_source)

    anim_sequence = asset_tools.create_asset(f'{source_texture.get_name()}', sequence_package,
                                             PaperZDAnimSequence_Flipbook.static_class(), factory)
    if not isinstance(anim_sequence, PaperZDAnimSequence):
        raise RuntimeError(INVALID_ASSET_ERROR)

    anim_data_source = Array(PaperFlipbook)
    anim_data_source.append(flipbooks[3])
    anim_data_source.append(flipbooks[2])
    anim_data_source.append(flipbooks[0])
    anim_data_source.append(flipbooks[1])

    anim_sequence.set_editor_property('directional_sequence', True)
    anim_sequence.set_editor_property('anim_data_source', anim_data_source)
    anim_sequence.set_editor_property('category', Name('Walking'))

    return anim_sequence
