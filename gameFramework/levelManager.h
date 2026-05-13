#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H


#include <string>
#include <unordered_map>
#include <utility>
#include <string_view>
#include <vector>
#include <SDL3/SDL_render.h>
#include "../utils/file.h"
#include "../sdlUtils/color.h"
#include "../sdlUtils/fRect.h"
#include "../entities/iBlock.h"
#include "../entities/basicBlock.h"
#include "../entities/hardBlock.h"
#include "../entities/wallBlock.h"
#include "../entities/ghostBlock.h"
#include "../entities/ballBlock.h"
#include "../entities/ball.h"
#include "../entities/player.h"
#include "../framework/colors.h"
#include "../framework/framework.h"


class LevelManager {
public:
    using LevelObjects = std::pair<std::vector<Ball*>&, std::vector<IBlock*>&>;

private:
    int m_levelAreaX {};
    int m_levelAreaY {};
    int m_tileWidth {};
    int m_tileHeight {};
    int m_blockWidth {};
    int m_blockHeight {};


    File m_fileReader{};
    const char m_tileDelim {','};
    const char m_tileSymbolDelim {'-'};
    const char* m_emptySpaceSymbol {"O"};
    std::vector<std::string> m_levelFilePaths {
        {"./levels/level_1.txt"},
        {"./levels/level_2.txt"}
    };


    std::vector<IBlock*> m_blocks{};

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


    static constexpr std::string m_basicBlockSymbol{"B"};
    static constexpr std::string m_hardBlockSymbol{"HB"};
    static constexpr std::string m_wallBlockSymbol{"WB"};
    static constexpr std::string m_ghostBlockSymbol{"GB"};
    static constexpr std::string m_ballBlockSymbol{"BB"};

    static constexpr int basicBlockScore {5};
    static constexpr int hardBlockScore {20};
    static constexpr int ballBlockScore {10};
    static constexpr int basicBlockHealth {1};
    static constexpr int hardBlockHealth {2};
    static constexpr int blockX {0};
    static constexpr int blockY {0};
    std::unordered_map<std::string, IBlock*> m_blockMap{
        {
            m_basicBlockSymbol, 
            new BasicBlock{blockX, blockY, Framework::images.basicBlockImg, Color{}, basicBlockHealth, basicBlockScore}
        },
        {
            m_hardBlockSymbol,
            new HardBlock{
                blockX, blockY, 
                Framework::images.hardBlockImg, 
                Framework::images.hardBlockHitImg, 
                Color{}, hardBlockHealth, hardBlockScore
            }
        },
        {
            m_wallBlockSymbol,
            new WallBlock{blockX, blockY, Framework::images.wallBlockImg, Color{}}
        },
        {
            m_ghostBlockSymbol,
            new GhostBlock{
                blockX, blockY, 
                Framework::images.ghostBlockMask, 
                Framework::images.basicBlockImg, 
                Color{}, basicBlockHealth, hardBlockScore
            }
        },
        {
            m_ballBlockSymbol,
            new BallBlock{blockX, blockY, Framework::images.ballBlockImg, Color{}, basicBlockHealth, ballBlockScore, &m_balls}
        }
    };


    int m_currentLevel {0};


public:
    LevelManager(int x, int y, int tileWidth, int blockWidth, int blockHeight, int tileHeight, Player* playerPtr);

    ~LevelManager();
    LevelManager(const LevelManager&) = delete;
    LevelManager(LevelManager&&) = delete;
    LevelManager& operator=(const LevelManager&) = delete;
    LevelManager& operator=(LevelManager&&) = delete;

    void loadLevel(int levelNum);

    int getLevelNum() const;

    void updateLevel(SDL_Renderer* renderer, double deltaTime, const FRect& screenRect);

    LevelObjects getLevelObjects();

    void loadPlayerPaddle();
    

private:
    void readLevelFile(int filePathIdx, std::vector<std::vector<std::string>>& level);

    void parseBlock(std::string_view token, int row, int col);

    std::pair<float, float> calcPixelCoords(int row, int col);

    void clearLevel();

    void clearDeadBlocks();

    void clearDeadBalls(const FRect& screenRect);

    void initializeBall();

    IBlock* getBlock(const std::string& blockSymbol);
};


#endif