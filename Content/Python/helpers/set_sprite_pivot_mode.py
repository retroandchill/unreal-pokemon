from unreal import AssetRegistryHelpers, Name, PaperSprite, EditorAssetLibrary, SpritePivotMode, ScopedSlowTask, Array


def change_pivot_point(package_name: str):
    asset_registry = AssetRegistryHelpers.get_asset_registry()
    assets = asset_registry.get_assets_by_path(Name(package_name))
    if assets is None:
        return

    with ScopedSlowTask(len(assets), "Update sprites") as slow_task:
        slow_task.make_dialog(True)

        to_save: Array[PaperSprite] = Array(PaperSprite)
        for asset in assets:
            if slow_task.should_cancel():
                break

            slow_task.enter_progress_frame(1, f'Processing: {asset.get_full_name()}')
            sprite = asset.get_asset()
            if not isinstance(sprite, PaperSprite):
                continue

            sprite.set_editor_property('PivotMode', SpritePivotMode.BOTTOM_CENTER)
            to_save.append(sprite)

            if len(assets) >= 100:
                EditorAssetLibrary.save_loaded_assets(to_save)
                to_save = Array(PaperSprite)


if __name__ == '__main__':
    change_pivot_point('/Game/Graphics/Pokemon/Back/_Source/')
    change_pivot_point('/Game/Graphics/Pokemon/BackShiny/_Source/')
    change_pivot_point('/Game/Graphics/Pokemon/Eggs/_Source/')