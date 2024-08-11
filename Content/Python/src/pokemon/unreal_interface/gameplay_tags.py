from typing import TypedDict


class GameplayTagData(TypedDict):
    TagName: str


class GameplayTagContainerData(TypedDict):
    GameplayTags: list[GameplayTagData]
    ParentTags: list[GameplayTagData]


def create_gameplay_tag_structure(prefix: str, tags: list[str]) -> GameplayTagContainerData:
    result = {
        "GameplayTags": [],
        "ParentTags": []
    }

    if len(tags) == 0:
        return result

    result["GameplayTags"].extend(map(lambda s: {'TagName': f'{prefix}.{s}'}, tags))
    parent_tags = prefix.split('.')
    for i in range(len(parent_tags), 0, -1):
        result["ParentTags"].append({'TagName': '.'.join(parent_tags[:i])})

    return result
