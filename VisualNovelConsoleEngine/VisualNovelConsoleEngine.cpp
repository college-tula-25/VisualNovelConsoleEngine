#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <windows.h>
#include "ConsoleRender.h"
#include "InputManager.h"
#include "SceneLoader.h"

// ps диалоги и арты писал чат гпт)
int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    ConsoleRender renderer(80, 25);
    InputManager input;

    SceneData introScene;
    if (!SceneLoader::LoadScene("intro", introScene)) {
        std::cerr << "Критическая ошибка: Не удалось загрузить файлы сцены 'intro'!" << std::endl;
        Sleep(3000);
        return 1;
    }

    std::vector<std::string> dialogueBox = {
        "==============================================================================",
        "|                                                                            |",
        "|                                                                            |",
        "|                                                                            |",
        "=============================================================================="
    };

    int currentStep = 0;
    int totalSteps = static_cast<int>(introScene.dialogueLines.size());
    bool running = true;

    input.Flush();

    while (running) {
        InputManager::Key key = input.GetPressedKey();

        if (key == InputManager::Key::Escape) {
            running = false;
        }

        if (key == InputManager::Key::Enter || key == InputManager::Key::Space) {
            currentStep++;
            if (currentStep >= totalSteps) {
                currentStep = 0;
            }
        }

        if (!running) break;

        const DialogueLine& currentDialogue = introScene.dialogueLines[currentStep];

        const std::vector<std::string>& activeSprite =
            (currentDialogue.artIndex < introScene.artAssets.size())
            ? introScene.artAssets[currentDialogue.artIndex]
            : introScene.artAssets[0];


        renderer.Clear();

        std::vector<std::string> backgroundBorder(14, "#                                                                          #");
        backgroundBorder[0] = std::string(76, '#');
        backgroundBorder[13] = std::string(76, '#');
        renderer.DrawArt(2, 1, backgroundBorder, 0x08, false);

        renderer.DrawArt(35, 6, activeSprite, 0x0B, true);

        renderer.DrawArt(2, 16, dialogueBox, 0x0E, false);

        renderer.DrawText(5, 17, currentDialogue.name + ":", 0x0F);
        renderer.DrawText(5, 19, currentDialogue.text, 0x07);

        renderer.Present();

        Sleep(16);
    }

    return 0;
}
