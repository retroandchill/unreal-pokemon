from unreal import AssetRegistryHelpers, Name, ScopedSlowTask, Array, PaperFlipbook, EditorAssetLibrary


def change_pivot_point(package_name: str):
    asset_registry = AssetRegistryHelpers.get_asset_registry()
    assets = asset_registry.get_assets_by_path(Name(package_name))
    if assets is None:
        return

    with ScopedSlowTask(len(assets), "Update sprites") as slow_task:
        slow_task.make_dialog(True)

        to_save: Array[PaperFlipbook] = Array(PaperFlipbook)
        for asset in assets:
            if slow_task.should_cancel():
                break

            slow_task.enter_progress_frame(1, f'Processing: {asset.get_full_name()}')
            sprite = asset.get_asset()
            if not isinstance(sprite, PaperFlipbook):
                continue

            sprite.set_editor_property('FramesPerSecond', 12)
            to_save.append(sprite)

            if len(assets) >= 200:
                EditorAssetLibrary.save_loaded_assets(to_save)
                to_save = Array(PaperFlipbook)


if __name__ == '__main__':
    change_pivot_point('/Game/Graphics/Pokemon/Front/')
    change_pivot_point('/Game/Graphics/Pokemon/FrontShiny/')
    change_pivot_point('/Game/Graphics/Pokemon/Back/')
    change_pivot_point('/Game/Graphics/Pokemon/BackShiny/')
    change_pivot_point('/Game/Graphics/Pokemon/Eggs/')
