
#include <AEEngine.h>
#include <SceneManager.h>
#include <InputManager.h>
#include <PauseManager.h>

#include <SceneGameLevel.h>
#include <SceneSplashscreen.h>

namespace SceneManager {

    // Scene states
    SCENES_ENUM previous, current, next;

    // Scenes
    Scene* currentScene;

    SceneGameLevel gameLevel;
    // SceneMainMenu mainMenu;
    // SceneSettings settings;
    
    #pragma region Forward Declarations
    void UpdatePausedState(bool newPausedState);
    #pragma endregion

    double fixedDT = 0.02, elapsedfixedDT = 0;

    bool isPaused;

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

                if (!isPaused) {
                    elapsedfixedDT += AEFrameRateControllerGetFrameTime();
                }
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

        Free();
    }

    void Initialize(SCENES_ENUM startingScene) {
        current = previous = next = startingScene;

        PauseManager::onTogglePause.Subscribe(UpdatePausedState);

        SwitchScene();
        SceneManagerLoop();
    }

    void Free() {
        PauseManager::onTogglePause.UnsubscribeAll();
    }

    void UpdatePausedState(bool newPausedState) {
        isPaused = newPausedState;
    }
}
