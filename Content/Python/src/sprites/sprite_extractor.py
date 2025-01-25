from typing import Optional

from unreal import Texture2D, PaperSprite, PaperSpriteFactory, AssetToolsHelpers, Vector2D, SpritePivotMode, \
    PaperFlipbook, PaperFlipbookFactory, Array, PaperFlipbookKeyFrame, PaperZDAnimationSource, PaperZDAnimSequence, \
    Object, PaperZDAnimSequenceFactory, PaperZDAnimSequence_Flipbook, PaperZDEditorHelpers, Name, \
    SimpleFlipbook, SimpleFlipbookFactory, SimpleFlipbookKeyFrame

INVALID_ASSET_ERROR = 'Invalid asset type created'

DIRECTIONS = [2, 4, 6, 8]


def convert_filename_to_package_name(filename: str) -> str:
    # Replace windows path separators
    filename = filename.replace('\\', '/')
    paths = filename.split('/')
    return f'{"/".join(paths[:-1])}/_Source/{paths[-1]}'


def get_package_name(texture: Object) -> str:
    return texture.get_path_name().removesuffix(f'{texture.get_name()}.{texture.get_name()}')


def get_parent_package(package_name: str) -> str:
    return f'{"/".join(package_name.split("/")[:-2])}/'


def create_sprites_from_texture(source_texture: Texture2D, columns: int,
                                grid_size: Optional[int] = None) -> list[PaperSprite]:
    asset_tools = AssetToolsHelpers.get_asset_tools()
    factory = PaperSpriteFactory()
    texture_package = get_package_name(source_texture)

    cell_size = Vector2D(source_texture.blueprint_get_size_x() / columns,
                         source_texture.blueprint_get_size_y())
    sprites = []
    for i in range(columns):
        new_sprite = asset_tools.create_asset(f'{source_texture.get_name()}_Sprite_{len(sprites)}',
                                              texture_package, PaperSprite.static_class(), factory)
        if not isinstance(new_sprite, PaperSprite):
            raise RuntimeError(INVALID_ASSET_ERROR)

        offset = Vector2D(i * cell_size.x, 0)

        compile_sprite(new_sprite, source_texture, cell_size, offset, grid_size)
        sprites.append(new_sprite)

    return sprites


def create_sprites_from_sprite_sheet(source_texture: Texture2D, frames: int,
                                     rows: int, columns: int) -> list[PaperSprite]:
    asset_tools = AssetToolsHelpers.get_asset_tools()
    factory = PaperSpriteFactory()
    texture_package = get_package_name(source_texture)

    cell_size = Vector2D(source_texture.blueprint_get_size_x() / columns,
                         source_texture.blueprint_get_size_y() / rows)

    sprites = []
    for i in range(frames):
        new_sprite = asset_tools.create_asset(f'{source_texture.get_name()}_Sprite_{len(sprites)}',
                                              texture_package, PaperSprite.static_class(), factory)
        if not isinstance(new_sprite, PaperSprite):
            raise RuntimeError(INVALID_ASSET_ERROR)

        offset = Vector2D((i % columns) * cell_size.x, (i // columns) * cell_size.y)

        compile_sprite(new_sprite, source_texture, cell_size, offset)
        sprites.append(new_sprite)

    return sprites


def compile_sprite(new_sprite: PaperSprite, source_texture: Texture2D, cell_size: Vector2D, offset: Vector2D,
                   grid_size: Optional[int] = None) -> None:
    new_sprite.set_editor_property('source_texture', source_texture)
    new_sprite.set_editor_property('source_uv', offset)
    new_sprite.set_editor_property('source_dimension', cell_size)
    if grid_size is not None:
        new_sprite.set_editor_property('pivot_mode', SpritePivotMode.CUSTOM)
        new_sprite.set_editor_property('custom_pivot_point', Vector2D(offset.x + cell_size.x / 2,
                                                                      offset.y + cell_size.y - grid_size / 2))
    else:
        new_sprite.set_editor_property('pivot_mode', SpritePivotMode.CENTER_CENTER)


def create_directional_sprites_from_texture(source_texture: Texture2D, columns: int,
                                            grid_size: Optional[int] = None) -> list[PaperSprite]:
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

            compile_sprite(new_sprite, source_texture, cell_size, offset, grid_size)
            sprites.append(new_sprite)

    return sprites


def compile_sprites_into_flipbook(source_texture: Texture2D, sprites: list[PaperSprite],
                                  frame_rate: float, frame_order: Optional[list[int]] = None) -> PaperFlipbook:
    asset_tools = AssetToolsHelpers.get_asset_tools()
    factory = PaperFlipbookFactory()
    texture_package = get_parent_package(get_package_name(source_texture))

    new_flipbook = asset_tools.create_asset(source_texture.get_name(), texture_package,
                                            PaperFlipbook.static_class(), factory)
    if not isinstance(new_flipbook, PaperFlipbook):
        raise RuntimeError(INVALID_ASSET_ERROR)

    key_frames: Array[PaperFlipbookKeyFrame] = Array(PaperFlipbookKeyFrame)
    if frame_order is None:
        frame_order = range(len(sprites))
    for i in frame_order:
        sprite = sprites[i]
        frame = PaperFlipbookKeyFrame()
        frame.set_editor_property('sprite', sprite)
        key_frames.append(frame)

    new_flipbook.set_editor_property('key_frames', key_frames)
    new_flipbook.set_editor_property('frames_per_second', frame_rate)
    return new_flipbook


def compile_sprites_into_directional_flipbooks(source_texture: Texture2D, sprites: list[PaperSprite], columns: int,
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

def compile_texture_into_flipbook(source_texture: Texture2D, rows: int, columns: int,
                                  frame_rate: float, frame_order: Optional[list[tuple[int, int]]] = None,
                                  pivot_mode: SpritePivotMode = SpritePivotMode.CENTER_CENTER) -> SimpleFlipbook:
    asset_tools = AssetToolsHelpers.get_asset_tools()
    factory = SimpleFlipbookFactory()
    texture_package = get_parent_package(get_package_name(source_texture))

    new_flipbook = asset_tools.create_asset(source_texture.get_name(), texture_package,
                                            PaperFlipbook.static_class(), factory)
    if not isinstance(new_flipbook, PaperFlipbook):
        raise RuntimeError(INVALID_ASSET_ERROR)

    new_flipbook.set_editor_property('source_texture', source_texture)
    new_flipbook.set_editor_property('rows', rows)
    new_flipbook.set_editor_property('columns', columns)

    key_frames: Array[SimpleFlipbookKeyFrame] = Array(SimpleFlipbookKeyFrame)
    if frame_order is None:
        frame_order = map(lambda x: (x, 1), range(rows * columns))
    for index, run in frame_order:
        frame = SimpleFlipbookKeyFrame()
        frame.set_editor_property('index', index)
        frame.set_editor_property('frame_run', run)
        key_frames.append(frame)

    new_flipbook.set_editor_property('key_frames', key_frames)
    new_flipbook.set_editor_property('frames_per_second', frame_rate)
    new_flipbook.set_editor_property('pivot_mode', )
    return new_flipbook

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
