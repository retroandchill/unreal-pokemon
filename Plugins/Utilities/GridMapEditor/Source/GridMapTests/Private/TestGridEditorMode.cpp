#include "EditorModeManager.h"
#include "EditorModes.h"
#include "GridMapEditorMode.h"
#include "InteractiveToolManager.h"
#include "Misc/AutomationTest.h"

BEGIN_DEFINE_SPEC(TestGridEditorMode, "Unit Tests.Editor.Tools.GridMap.EditorMode",
                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

UEdMode *EditorMode;
UInteractiveToolManager *InteractiveToolManager;
UWorld *World;
AStaticMeshActor *PolygonMesh;

END_DEFINE_SPEC(TestGridEditorMode)

void TestGridEditorMode::Define()
{
    Describe(TEXT("EditTileMap"), [this] {
        BeforeEach([this] {
            // Entering Extrude Polygon Tool
            GLevelEditorModeTools().ActivateMode(FGridMapEditorMode::EM_GridMapEditorModeId);
            EditorMode = GLevelEditorModeTools().GetActiveScriptableMode(FGridMapEditorMode::EM_GridMapEditorModeId);
            if (EditorMode != nullptr)
            {
                InteractiveToolManager = EditorMode->GetToolManager();
                World = EditorMode->GetWorld();
            }
        });

        It("Editor mode should not allow selection", [this] {
            auto Actor = World->SpawnActor<AActor>();
            TestFalse(TEXT("Editor mode should not allow selection"), EditorMode->IsSelectionAllowed(Actor, true));
            Actor->Destroy();
        });

        AfterEach([this] {
            // Resetting the interface out of Modeling Tools
            InteractiveToolManager->DeactivateTool(EToolSide::Left, EToolShutdownType::Completed);
            GLevelEditorModeTools().ActivateMode(FBuiltinEditorModes::EM_Default);
        });
    });
}