import csv

from sprites.sprite_extractor import convert_filename_to_package_name, compile_texture_into_flipbook
from unreal import Texture2D, ScopedSlowTask, EditorAssetLibrary, PaperZDEditorHelpers, SpritePivotMode, \
    MaterialInterface


def execute(base_package: str, manifest_file: str, material: MaterialInterface):
    """
    Perform the creation of the sprites for the given icon sets.
    """
    with open(manifest_file, "r") as manifest_file:
        csv_reader = csv.reader(manifest_file)
        data: list[tuple[str, int, int, int, float, list[tuple[int, int]]]] = []
        next(csv_reader, None)
        for row in csv_reader:
            data.append((row[0], int(row[1]), int(row[2]), int(row[3]), float(row[4]), eval(row[5])))

    with ScopedSlowTask(len(data), "Importing Sprites") as slow_task:
        slow_task.make_dialog(True)
        for texture, frames, rows, columns, frame_rate, frame_data in data:
            if slow_task.should_cancel():
                break

            slow_task.enter_progress_frame(1, f'Processing: {texture}')
            texture = convert_filename_to_package_name(f'{base_package}/{texture}')
            source_texture = EditorAssetLibrary.load_asset(texture)
            if not isinstance(source_texture, Texture2D):
                continue

            PaperZDEditorHelpers.finish_loading_texture(source_texture)

            compile_texture_into_flipbook(source_texture, material, rows, columns, frame_rate, frame_data,
                                          SpritePivotMode.BOTTOM_CENTER)
