from unreal import Texture2D, Array, ScopedSlowTask

from sprites.sprite_extractor import compile_sprites_into_flipbook, create_sprites_from_texture, sprite_exists


def execute(textures: Array[Texture2D], frame_rate: float):
    """
    Perform the creation of the sprites for the given icon sets.
    """
    with ScopedSlowTask(len(textures), "Importing Icons") as slow_task:
        slow_task.make_dialog(True)
        for texture in textures:
            if slow_task.should_cancel():
                break

            slow_task.enter_progress_frame(1, f'Processing: {texture.get_name()}')
            columns = int(texture.blueprint_get_size_x() / texture.blueprint_get_size_y())
            sprites = create_sprites_from_texture(texture, columns)
            compile_sprites_into_flipbook(texture, sprites, frame_rate)
