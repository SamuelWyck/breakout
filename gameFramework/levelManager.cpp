#include <SDL3/SDL_render.h>
#include "./levelManager.h"
#include "../utils/file.h"
#include "../utils/stringUtility.h"
#include "../entities/basicBlock.h"



LevelManager::LevelManager(int x, int y, int tileWidth, int tileHeight, Player* playerPtr) 
    : m_levelAreaX{x}, m_levelAreaY{y}, m_tileWidth{tileWidth}, m_tileHeight{tileHeight}, m_playerPtr{playerPtr} 
    {
};


LevelManager::~LevelManager() {
    clearLevel();
};


void LevelManager::loadLevel(int filePathIdx) {
    clearLevel();

    std::vector<std::vector<std::string>> level {};
    readLevelFile(filePathIdx, level);

    int numRows {static_cast<int>(level.size())};
    int numCols {static_cast<int>(level[0].size())};
    for (int row {0}; row < numRows; row += 1) {
        for (int col {0}; col < numCols - 1; col += 1) { // numCols - 1 to ignore \n char at the end
            if (level[row][col] == m_emptySpaceSymbol) {
                continue;
            }

            parseBlock(level[row][col], row, col);
        }
    }

    initializeBall();
};


void LevelManager::parseBlock(std::string_view token, int row, int col) {
    constexpr int blockSymbolIdx {0};
    constexpr int colorSymbolIdx {1};
    std::vector<std::string> tokenParts{StringUtils::split(token, m_tileSymbolDelim)};

    const std::string& blockSymbol{tokenParts[blockSymbolIdx]};
    const std::string& colorSymbol{(tokenParts.size() > 1) ? tokenParts[colorSymbolIdx] : m_defaultColorSymbol};


    BasicBlock* blockPtr {new BasicBlock{m_blockMap[blockSymbol]}};
    auto [x, y] {calcPixelCoords(row, col)};
    blockPtr->setTopLeft(x, y);
    blockPtr->setColor(m_colorMap[colorSymbol]);

    m_blocks.push_back(blockPtr);
};


void LevelManager::initializeBall() {
    constexpr float ballSpeed {0};
    constexpr float ballCenterCoord{0};
    m_balls.push_back(new Ball{ballCenterCoord, ballCenterCoord, m_ballRadius, ballSpeed, ballSpeed});
    m_playerPtr->loadBall(m_balls[0]);
};


std::pair<float, float> LevelManager::calcPixelCoords(int row, int col) {
    float x {(col * m_tileWidth) + static_cast<float>(m_levelAreaX)};
    float y {(row * m_tileHeight) + static_cast<float>(m_levelAreaY)};
    return std::pair<float, float>{x, y};
};


void LevelManager::readLevelFile(int filePathIdx, std::vector<std::vector<std::string>>& level) {
    m_fileReader.setFilePath(m_levelFilePaths[filePathIdx]);
    std::vector<std::string> fileLines{m_fileReader.readLines()};

    for (const std::string& line : fileLines) {
        level.push_back(StringUtils::split(line, m_tileDelim));
    }
};


void LevelManager::clearLevel() {
    m_playerPtr->launchBall();

    for (Ball* ballPtr : m_balls) {
        delete ballPtr;
    }
    m_balls.clear();

    for (BasicBlock* blockPtr : m_blocks) {
        delete blockPtr;
    }
    m_blocks.clear();
};


void LevelManager::updateLevel(SDL_Renderer* renderer, double deltaTime) {
    for (Ball* ballPtr : m_balls) {
        ballPtr->update(renderer, deltaTime);
    }

    bool removeBlock {false};
    for (BasicBlock* blockPtr : m_blocks) {
        if (blockPtr->isDead()) {
            removeBlock = true;
            continue;
        }

        blockPtr->update(renderer);
    }

    if (removeBlock) {
        clearDeadBlocks();
    }
};


void LevelManager::clearDeadBlocks() {
    std::vector<BasicBlock*> liveBlocks{};
    for (BasicBlock* blockPtr : m_blocks) {
        if (blockPtr->isDead()) {
            delete blockPtr;
            blockPtr = nullptr;
        } else {
            liveBlocks.push_back(blockPtr);
        }
    }

    m_blocks = liveBlocks;
};


LevelManager::LevelObjects LevelManager::getLevelObjects() {
    return {m_balls, m_blocks};
};