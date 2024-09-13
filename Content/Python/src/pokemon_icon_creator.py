from typing import Union

from unreal import Texture2D, Array, ScopedSlowTask

from sprites.sprite_extractor import compile_sprites_into_flipbook, create_sprites_from_texture


def execute(textures: Union[Array[Texture2D], list[Texture2D]], columns: int, frame_rate: float):
    """
    Perform the creation of the sprites for the given icon sets.
    """
    with ScopedSlowTask(len(textures), "Importing Icons") as slow_task:
        slow_task.make_dialog(True)
        for texture in textures:
            if slow_task.should_cancel():
                break

            slow_task.enter_progress_frame(1, f'Processing: {texture.get_name()}')
            sprites = create_sprites_from_texture(texture, columns)
            compile_sprites_into_flipbook(texture, sprites, frame_rate)
