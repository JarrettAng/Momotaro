
#include <SceneManager.h>
#include <SceneGameLevel.h>

namespace SceneManager {

    // Scene states
    SCENES_ENUM previous, current, next;

    // Scenes
    Scene* currentScene;

    SceneGameLevel gameLevel;
    // SceneMainMenu mainMenu;
    // SceneSettings settings;

    float fixedDT = 0.02, elapsedfixedDT = 0;

    void LoadScene(SCENES_ENUM nextScene) {
        next = nextScene;
    }

    void SwitchScene() {
        switch (current) {
        case GAME_LEVEL:
            currentScene = &gameLevel;
            break;
        }
    }

    void Initialize(SCENES_ENUM startingScene) {
        current = previous = next = startingScene;
        SwitchScene();

        while (current != QUIT) {
            if (current == RESTART) {
                next = current = previous;
            }
            else {
                SwitchScene();
                currentScene->Load();
            }

            currentScene->Initialize();

            //Game loop
            while (current == next) {
                AEFrameRateControllerStart();

                // Input_Handle();
                currentScene->Update();

                elapsedfixedDT += AEFrameRateControllerGetFrameTime();
                if (elapsedfixedDT > fixedDT) {
                    elapsedfixedDT = 0;
                    currentScene->FixedUpdate();
                }

                currentScene->Draw();

                AEFrameRateControllerEnd();
            }

            currentScene->Free();

            if (next != RESTART) {
                currentScene->Unload();
            }

            previous = current;
            current = next;
        }
    }
}
