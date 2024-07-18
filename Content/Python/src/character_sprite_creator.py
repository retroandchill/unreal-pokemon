from unreal import Set, Texture2D, Array

from sprites.sprite_extractor import create_sprites_from_texture


def execute(textures: Array[Texture2D], columns: int, grid_size: int):
    for texture in textures:
        create_sprites_from_texture(texture, columns, grid_size)
