from unreal import BlueprintFactory, Blueprint, EditorAssetLibrary, AssetToolsHelpers, Array, BattleStatChangeValue, \
    Name, get_default_object

STAT_STAGE_NAMES: dict[str, str] = {
    'ATTACK': 'Attack',
    'DEFENSE': 'Defense',
    'SPECIAL_ATTACK': 'SpAtk',
    'SPECIAL_DEFENSE': 'SpDef',
    'SPEED': 'Speed',
    'ACCURACY': 'Accuracy',
    'EVASION': 'Evasion'
}


def create_stat_stage_object(bp_factory: BlueprintFactory, prefix: str, stat_id: str, stages: int):
    asset_tools = AssetToolsHelpers.get_asset_tools()
    created_class = asset_tools.create_asset(f'Move_{prefix}{STAT_STAGE_NAMES[stat_id]}{stages}',
                                             '/Game/Blueprints/Battle/Moves/FunctionCodes',
                                             None,
                                             bp_factory)
    if isinstance(created_class, Blueprint):
        default_object = get_default_object(created_class.generated_class())

        stats_to_change: Array[BattleStatChangeValue] = Array(BattleStatChangeValue)
        stat_change = BattleStatChangeValue()
        stat_change.set_editor_property('StatID', Name(stat_id))
        stat_change.set_editor_property('Change', stages)
        stats_to_change.append(stat_change)
        default_object.set_editor_property('StatsToChange', stats_to_change)

        EditorAssetLibrary.save_loaded_asset(created_class)


def create_stat_stage_classes(prefix: str, base_class: str):
    parent_class = EditorAssetLibrary.load_blueprint_class('/Game/Blueprints/Battle/Moves/Base/{0}.{0}'.format(base_class))

    factory = BlueprintFactory()
    factory.set_editor_property('ParentClass', parent_class)
    for stat_id in STAT_STAGE_NAMES:
        for i in range(1, 4):
            create_stat_stage_object(factory, prefix, stat_id, i)


if __name__ == '__main__':
    create_stat_stage_classes('RaiseUser', 'Move_StatUp')
    create_stat_stage_classes('LowerUser', 'Move_StatDown')
    create_stat_stage_classes('LowerTarget', 'Move_TargetStatDown')



