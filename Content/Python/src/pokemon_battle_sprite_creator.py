import csv

from unreal import Texture2D, Array, ScopedSlowTask, EditorAssetLibrary

from sprites.sprite_extractor import compile_sprites_into_flipbook, create_sprites_from_texture, sprite_exists, \
    convert_filename_to_package_name, create_sprites_from_sprite_sheet


def execute(base_package: str, manifest_file: str, frame_rate: float):
    """
    Perform the creation of the sprites for the given icon sets.
    """
    with open(manifest_file, "r") as manifest_file:
        csv_reader = csv.reader(manifest_file)
        data = []
        next(csv_reader, None)
        for row in csv_reader:
            data.append((row[0], int(row[1]), int(row[2]), int(row[3])))

    with ScopedSlowTask(len(data), "Importing Icons") as slow_task:
        slow_task.make_dialog(True)
        for texture, frames, rows, columns  in data:
            if slow_task.should_cancel():
                break

            slow_task.enter_progress_frame(1, f'Processing: {texture}')
            texture = convert_filename_to_package_name(f'{base_package}{texture}')
            source_texture = EditorAssetLibrary.load_asset(texture)
            if not isinstance(source_texture, Texture2D):
                continue

            sprites = create_sprites_from_sprite_sheet(source_texture, frames, rows, columns)
            compile_sprites_into_flipbook(source_texture, sprites, frame_rate)
