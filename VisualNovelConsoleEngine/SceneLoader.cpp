#include "SceneLoader.h"

bool SceneLoader::LoadScene(const std::string& sceneName, SceneData& outScene)
{
    outScene.artAssets.clear();
    outScene.dialogueLines.clear();

    std::string artPath = "assets/" + sceneName + "/" + sceneName + "_art.txt";
    std::string textPath = "assets/" + sceneName + "/" + sceneName + "_text.txt";

    //потом поменять на свои ошибки
    if (!ParseSprites(artPath, outScene.artAssets)) {
        std::cerr << "[Ошибка] Не удалось загрузить спрайты: " << artPath << std::endl;
        return false;
    }

    if (!ParseDialogues(textPath, outScene.dialogueLines)) {
        std::cerr << "[Ошибка] Не удалось загрузить реплики: " << textPath << std::endl;
        return false;
    }

    return true;
}

bool SceneLoader::ParseSprites(const std::string& path, std::vector<std::vector<std::string>>& outArtAssets) {
    std::ifstream file(path);
    if (!file.is_open()) return false;

    std::vector<std::string> currentSprite;
    std::string line;

    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (line.empty()) {
            if (!currentSprite.empty()) {
                outArtAssets.push_back(currentSprite);
                currentSprite.clear();
            }
        }
        else {
            currentSprite.push_back(line);
        }
    }

    if (!currentSprite.empty()) {
        outArtAssets.push_back(currentSprite);
    }

    file.close();
    return true;
}

bool SceneLoader::ParseDialogues(const std::string& path, std::vector<DialogueLine>& outDialogues)
{
    std::ifstream file(path);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (line.back() == '\r') line.pop_back();

        std::stringstream ss(line);
        std::string name, text, indexStr;

        // Парсим строку формата: Имя|Текст реплики|ИндексСпрайта
        if (std::getline(ss, name, '|') &&
            std::getline(ss, text, '|') &&
            std::getline(ss, indexStr, '|')) {

            DialogueLine dl;
            dl.name = name;
            dl.text = text;
            dl.artIndex = std::stoi(indexStr);

            outDialogues.push_back(dl);
        }
    }

    file.close();
    return true;
}
