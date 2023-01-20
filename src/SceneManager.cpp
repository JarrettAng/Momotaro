
#include <AEEngine.h>
#include <SceneManager.h>
#include <SceneGameLevel.h>
#include <InputManager.h>

namespace SceneManager {

    // Scene states
    SCENES_ENUM previous, current, next;

    // Scenes
    Scene* currentScene;

    SceneGameLevel gameLevel;
    // SceneMainMenu mainMenu;
    // SceneSettings settings;

    double fixedDT = 0.02, elapsedfixedDT = 0;

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

    void SceneManagerLoop() {
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
                AESysFrameStart();

                AEInputUpdate();
                InputManager::HandleInput();

                currentScene->Update();

                elapsedfixedDT += AEFrameRateControllerGetFrameTime();
                if (elapsedfixedDT > fixedDT) {
                    elapsedfixedDT = 0;
                    currentScene->FixedUpdate();
                }

                currentScene->Draw();

                AESysFrameEnd();
            }

            currentScene->Free();

            if (next != RESTART) {
                currentScene->Unload();
            }

            previous = current;
            current = next;
        }
    }

    void Initialize(SCENES_ENUM startingScene) {
        current = previous = next = startingScene;
        SwitchScene();

        SceneManagerLoop();
    }
}
