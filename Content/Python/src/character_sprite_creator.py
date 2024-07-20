from unreal import Set, Texture2D, Array, PaperZDAnimationSource, ScopedSlowTask

from sprites.sprite_extractor import create_sprites_from_texture, compile_sprites_into_flipbooks, \
    place_flipbooks_in_animation_source


def execute(textures: Array[Texture2D], columns: int, grid_size: int, frame_rate: float,
            anim_source: PaperZDAnimationSource):
    with ScopedSlowTask(len(textures), "Importing Charactgers") as slow_task:
        slow_task.make_dialog(True)
        for texture in textures:
            if slow_task.should_cancel():
                break

            slow_task.enter_progress_frame(1, f'Processing: {texture.get_name()}')
            sprites = create_sprites_from_texture(texture, columns, grid_size)
            flipbooks = compile_sprites_into_flipbooks(texture, sprites, columns, frame_rate)
            place_flipbooks_in_animation_source(texture, flipbooks, anim_source)
