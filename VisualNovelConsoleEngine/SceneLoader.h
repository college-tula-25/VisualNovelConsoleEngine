#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct DialogueLine
{
	std::string name{};
	std::string text{};
	size_t artIndex{};
};

struct SceneData {
	std::vector<std::vector<std::string>> artAssets;
	std::vector<DialogueLine> dialogueLines;
};

class SceneLoader
{
public:
	static bool LoadScene(const std::string& sceneName, SceneData& sceneData);

private:
	static bool ParseSprites(const std::string& path,
		std::vector<std::vector<std::string>>& outSprites);
	static bool ParseDialogues(const std::string& path,
		std::vector<DialogueLine>& outDialogues);
};
