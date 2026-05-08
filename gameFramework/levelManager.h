#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H


#include <string>
#include <unordered_map>
#include <utility>
#include <string_view>
#include <vector>
#include <SDL3/SDL_render.h>
#include "../utils/file.h"
#include "../utils/color.h"
#include "../entities/basicBlock.h"
#include "../entities/ball.h"
#include "../entities/player.h"
#include "../framework/colors.h"


class LevelManager {
public:
    using LevelObjects = std::pair<std::vector<Ball*>&, std::vector<BasicBlock*>&>;

private:
    int m_levelAreaX {};
    int m_levelAreaY {};
    int m_tileWidth {};
    int m_tileHeight {};

    File m_fileReader{};
    const char m_tileDelim {','};
    const char m_tileSymbolDelim {'-'};
    const char* m_emptySpaceSymbol {"O"};
    std::vector<std::string> m_levelFilePaths {
        {"./levels/level_1.txt"}
    };


    std::vector<BasicBlock*> m_blocks{};

    static constexpr float m_ballRadius {15};
    std::vector<Ball*> m_balls{};

    Player* m_playerPtr{};

    static constexpr std::string m_defaultColorSymbol{"b"};
    std::unordered_map<std::string, Color> m_colorMap{
        {"r", Colors::RED},
        {"o", Colors::ORANGE},
        {"y",  Colors::YELLOW},
        {"l", Colors::LIME},
        {"g", Colors::GREEN},
        {"b", Colors::BLUE}
    };


    static constexpr int basicBlockScore {5};
    static constexpr int basicBlockHealth {1};
    static constexpr int blockX {0};
    static constexpr int blockY {0};
    std::unordered_map<std::string, BasicBlock> m_blockMap{
        {"B", BasicBlock{blockX, blockY, Color{}, basicBlockHealth, basicBlockScore}}
    };



public:
    LevelManager(int x, int y, int tileWidth, int tileHeight, Player* playerPtr);

    ~LevelManager();
    LevelManager(const LevelManager&) = delete;
    LevelManager(LevelManager&&) = delete;
    LevelManager& operator=(const LevelManager&) = delete;
    LevelManager& operator=(LevelManager&&) = delete;

    void loadLevel(int filePathIdx);

    void updateLevel(SDL_Renderer* renderer, double deltaTime);

    LevelObjects getLevelObjects();


private:
    void readLevelFile(int filePathIdx, std::vector<std::vector<std::string>>& level);

    void parseBlock(std::string_view token, int row, int col);

    std::pair<float, float> calcPixelCoords(int row, int col);

    void clearLevel();

    void clearDeadBlocks();

    void initializeBall();
};


#endif