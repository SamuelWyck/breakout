#include <SDL3/SDL_render.h>
#include "./levelManager.h"
#include "../utils/file.h"
#include "../utils/stringUtility.h"
#include "../entities/basicBlock.h"
#include "../utils/fRect.h"
#include "../entities/hardBlock.h"
#include "../entities/wallBlock.h"



LevelManager::LevelManager(int x, int y, int tileWidth, int tileHeight, int blockWidth, int blockHeight, Player* playerPtr) 
    : m_levelAreaX{x}, 
    m_levelAreaY{y}, 
    m_tileWidth{tileWidth}, 
    m_tileHeight{tileHeight}, 
    m_blockWidth{blockWidth},
    m_blockHeight{blockHeight},
    m_playerPtr{playerPtr} {
};


LevelManager::~LevelManager() {
    clearLevel();

    for (auto& entry : m_blockMap) {
        const std::string& key {entry.first};
        delete m_blockMap[key];
    }
};


void LevelManager::loadLevel(int levelNum) {
    m_currentLevel = levelNum;
    if (m_currentLevel >= static_cast<int>(m_levelFilePaths.size())) {
        m_currentLevel = 0;
    }

    clearLevel();

    std::vector<std::vector<std::string>> level {};
    readLevelFile(m_currentLevel, level);

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

    loadPlayerPaddle();
    if (m_currentLevel % 2 == 0) {
        m_playerPtr->healHealth();
    }
};


void LevelManager::parseBlock(std::string_view token, int row, int col) {
    constexpr int blockSymbolIdx {0};
    constexpr int colorSymbolIdx {1};
    std::vector<std::string> tokenParts{StringUtils::split(token, m_tileSymbolDelim)};

    const std::string& blockSymbol{tokenParts[blockSymbolIdx]};
    const std::string& colorSymbol{(tokenParts.size() > 1) ? tokenParts[colorSymbolIdx] : m_defaultColorSymbol};


    IBlock* blockPtr {getBlock(blockSymbol)};
    auto [x, y] {calcPixelCoords(row, col)};
    x += (m_tileWidth - m_blockWidth) / 2;
    y += (m_tileHeight - m_blockHeight) / 2;
    blockPtr->setTopleft(x, y);
    blockPtr->setColor(m_colorMap[colorSymbol]);

    m_blocks.push_back(blockPtr);
};


IBlock* LevelManager::getBlock(const std::string& blockSymbol) {
    IBlock* newBlock {nullptr};
    if (blockSymbol == m_basicBlockSymbol) {
        newBlock = new BasicBlock{m_blockMap[blockSymbol]};

    } else if (blockSymbol == m_hardBlockSymbol) {
        newBlock = new HardBlock{m_blockMap[blockSymbol]};

    } else if (blockSymbol == m_wallBlockSymbol) {
        newBlock = new WallBlock{m_blockMap[blockSymbol]};

    } else if (blockSymbol == m_ghostBlockSymbol) {
        newBlock = new GhostBlock{m_blockMap[blockSymbol]};

    } else if (blockSymbol == m_ballBlockSymbol) {
        newBlock = new BallBlock{m_blockMap[blockSymbol]};
        
    }

    return newBlock;
};


void LevelManager::initializeBall() {
    constexpr float ballSpeed {0};
    constexpr float ballCenterCoord{0};
    m_balls.push_back(
        new Ball{Framework::images.ballImg, ballCenterCoord, ballCenterCoord, m_ballRadius, ballSpeed, ballSpeed}
    );
    m_playerPtr->loadBall(m_balls[0]);
};


void LevelManager::loadPlayerPaddle() {
    initializeBall();
    m_playerPtr->resetPosition();
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

    for (IBlock* blockPtr : m_blocks) {
        delete blockPtr;
    }
    m_blocks.clear();
};


void LevelManager::updateLevel(SDL_Renderer* renderer, double deltaTime, const FRect& screenRect) {
    bool removeBall {false};
    for (Ball* ballPtr : m_balls) {
        if (ballPtr->m_rect.y() >= screenRect.bottom()) {
            removeBall = true;
            continue;
        }

        ballPtr->update(renderer, deltaTime);
    }


    bool removeBlock {false};
    for (IBlock* blockPtr : m_blocks) {
        if (blockPtr->isDead()) {
            removeBlock = true;
            continue;
        }

        blockPtr->update(renderer);
    }


    if (removeBlock) {
        clearDeadBlocks();
    }
    if (removeBall) {
        clearDeadBalls(screenRect);
    }
};


void LevelManager::clearDeadBlocks() {
    std::vector<IBlock*> liveBlocks{};
    for (IBlock* blockPtr : m_blocks) {
        if (blockPtr->isDead()) {
            delete blockPtr;
        } else {
            liveBlocks.push_back(blockPtr);
        }
    }

    m_blocks = liveBlocks;
};


void LevelManager::clearDeadBalls(const FRect& screenRect) {
    std::vector<Ball*> liveBalls{};
    for (Ball* ballPtr : m_balls) {
        if (ballPtr->m_rect.y() >= screenRect.bottom()) {
            delete ballPtr;
        } else {
            liveBalls.push_back(ballPtr);
        }
    }

    m_balls = liveBalls;
};


LevelManager::LevelObjects LevelManager::getLevelObjects() {
    return {m_balls, m_blocks};
};


int LevelManager::getLevelNum() const {
    return m_currentLevel;
};