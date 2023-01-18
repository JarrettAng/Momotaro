
#include <SceneManager.h>
#include <SceneGameLevel.h>

Scene *currentScene;

// Scenes
SceneGameLevel gameLevel;
// SceneMainMenu mainMenu;
// SceneSettings settings;

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
            // Input_Handle();
            currentScene->Update();
            currentScene->Draw();
        }

        currentScene->Free();

        if (next != RESTART) {
            currentScene->Unload();
        }

        previous = current;
        current = next;
    }
}
