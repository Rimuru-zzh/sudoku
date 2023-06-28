#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <random>

const int GRID_SIZE = 9;       // 数独格子的总大小
const int SUBGRID_SIZE = 3;    // 子网格的大小

typedef std::vector<std::vector<int>> SudokuGrid;

// 寻找数独中的空格（值为0）的位置
bool findEmptyCell(const SudokuGrid& grid, int& row, int& col) {
    for (row = 0; row < GRID_SIZE; row++) {
        for (col = 0; col < GRID_SIZE; col++) {
            if (grid[row][col] == 0) {
                // 找到空格子
                return true;
            }
        }
    }
    // 没有空格子
    return false;
}

// 解数独的算法实现
bool isSafe(const SudokuGrid& grid, int row, int col, int num) {
    // 检查同一行是否存在重复数字
    for (int c = 0; c < GRID_SIZE; c++) {
        if (grid[row][c] == num) {
            return false;
        }
    }

    // 检查同一列是否存在重复数字
    for (int r = 0; r < GRID_SIZE; r++) {
        if (grid[r][col] == num) {
            return false;
        }
    }

    // 检查同一个九宫格是否存在重复数字
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (int r = startRow; r < startRow + 3; r++) {
        for (int c = startCol; c < startCol + 3; c++) {
            if (grid[r][c] == num) {
                return false;
            }
        }
    }

    // 数字填充合法
    return true;
}

bool solveSudoku(SudokuGrid& grid) {
    int row, col;
    if (!findEmptyCell(grid, row, col)) {
        // 所有格子都已填满，数独问题解决
        return true;
    }

    // 尝试填充数字 1 到 9
    for (int num = 1; num <= 9; num++) {
        if (isSafe(grid, row, col, num)) {
            // 填充当前格子
            grid[row][col] = num;

            // 递归尝试填充下一个格子
            if (solveSudoku(grid)) {
                return true;  // 找到解决方案
            }

            // 回溯，将当前格子重置为0
            grid[row][col] = 0;
        }
    }

    // 无法找到合适的数字填充，回溯到上一步
    return false;
}


// 生成一个随机的数独终局

SudokuGrid generateCompletedSudoku() {
    SudokuGrid grid(GRID_SIZE, std::vector<int>(GRID_SIZE));

    // 初始化种子
    std::random_device rd;
    std::mt19937 gen(rd());
    // 使用 1-9 的随机排列填充第一行
    std::vector<int> nums = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::shuffle(nums.begin(), nums.end(), gen);
    for (int i = 0; i < GRID_SIZE; i++) {
        grid[0][i] = nums[i];
    }

    // 通过排列第一行的数字，递归填充剩余格子
    solveSudoku(grid);

    return grid;
}



// 将数独终局写入文件
void writeSudokuToFile(const SudokuGrid& grid, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (int row = 0; row < GRID_SIZE; row++) {
            for (int col = 0; col < GRID_SIZE; col++) {
                file << grid[row][col] << " ";
            }
            file << std::endl;
        }
        file.close();
    }
    else {
        std::cerr << "无法打开文件进行写入: " << filename << std::endl;
    }
}
void writeSudokuToFile(const SudokuGrid& grid, std::ofstream& file) {
    for (const auto& row : grid) {
        for (const auto& num : row) {
            file << num << " ";
        }
        file << std::endl;
    }
}

// 从文件中读取数独终局
void solveSudokuPuzzles(const std::string& puzzleFile) {
    std::ifstream inputFile(puzzleFile);
    std::ofstream outputFile("output.txt");

    if (inputFile.is_open() && outputFile.is_open()) {
        std::string line;
        SudokuGrid grid;

        while (std::getline(inputFile, line)) {
            if (line.empty()) {
                continue; // 跳过空行
            }

            if (line == "===") {
                if (!grid.empty()) {
                    // 解决当前数独谜题
                    solveSudoku(grid);

                    // 将解决后的数独写入输出文件
                    writeSudokuToFile(grid, outputFile);
                    outputFile << std::endl << "===" << std::endl << std::endl;

                    // 清空当前数独谜题
                    grid.clear();
                }
            }
            else {
                std::vector<int> row;
                std::string numStr;
                for (char c : line) {
                    if (std::isdigit(c)) {
                        numStr += c;
                    }
                    else if (!numStr.empty()) {
                        int num = std::stoi(numStr);
                        row.push_back(num);
                        numStr.clear();
                    }
                }

                // 添加当前行到数独谜题
                if (!row.empty()) {
                    grid.push_back(row);
                }
            }
        }

        // 处理最后一个数独谜题
        if (!grid.empty()) {
            solveSudoku(grid);
            writeSudokuToFile(grid, outputFile);
        }

        inputFile.close();
        outputFile.close();
    }
    else {
        std::cerr << "无法打开文件进行读取或写入: " << puzzleFile << std::endl;
    }
}



// 生成数独游戏并写入文件
bool hasUniqueSolution(const SudokuGrid& grid) {
    SudokuGrid copyGrid = grid;
    return solveSudoku(copyGrid);
}

void generateSudokuGames(int numGames, int minHoles, int maxHoles, bool unique, const std::string& filename) {
    std::vector<SudokuGrid> baseGrids;
    std::ifstream inputFile("sudoku_syukyoku.txt");

    if (inputFile.is_open()) {
        std::string line;
        SudokuGrid grid;
        while (std::getline(inputFile, line)) {
            if (line == "===") {
                baseGrids.push_back(grid);
                grid.clear();
            }
            else {
                std::vector<int> row;
                int num = 0;
                int pos = 0;
                for (char c : line) {
                    if (std::isdigit(c)) {
                        num = num * 10 + (c - '0');
                    }
                    else if (num > 0) {
                        row.push_back(num);
                        num = 0;
                        pos++;
                    }
                    if (pos == GRID_SIZE) {
                        grid.push_back(row);
                        row.clear();
                        pos = 0;
                    }
                }
            }
        }
        inputFile.close();
    }
    else {
        std::cerr << "无法打开文件进行读取: " << "sudoku_syukyoku.txt" << std::endl;
        return;
    }

    std::ofstream outputFile(filename);
    if (outputFile.is_open()) {
        for (int i = 0; i < numGames; i++) {
            SudokuGrid gameGrid;
            int randomIndex = std::rand() % baseGrids.size();
            gameGrid = baseGrids[randomIndex];

            int numHoles = std::rand() % (maxHoles - minHoles + 1) + minHoles;
            for (int j = 0; j < numHoles; j++) {
                int row = std::rand() % 9;
                int col = std::rand() % 9;
                gameGrid[row][col] = 0;
            }

            if (unique && !hasUniqueSolution(gameGrid)) {
                // 如果要求唯一解且生成的谜题没有唯一解，则重新生成谜题
                i--;
                continue;
            }

            writeSudokuToFile(gameGrid, outputFile);

            if (i < numGames - 1) {
                outputFile << "===" << std::endl;
            }
        }
        outputFile.close();
    }
    else {
        std::cerr << "无法打开文件进行写入: " << filename << std::endl;
    }
}


void generateSudokuGrids(int numGrids, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < numGrids; i++) {
            SudokuGrid grid = generateCompletedSudoku();
            for (const auto& row : grid) {
                for (const auto& num : row) {
                    file << num << " ";
                }
                file << std::endl;
            }

            if (i < numGrids - 1) {
                file << "===" << std::endl;
            }
        }
        file.close();
    }
    else {
        std::cerr << "无法打开文件进行写入: " << filename << std::endl;
    }
}

int main(int argc, char* argv[]) {
    // 设置随机种子
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // 检查命令行参数
    if (argc > 1) {
        std::string command = argv[1];

        if (command == "-c" && argc == 3) {
            // 处理生成数独终局的情况
            int numGrids = std::atoi(argv[2]);
            if (numGrids >= 1 && numGrids <= 1000000) {
                generateSudokuGrids(numGrids, "sudoku_syukyoku.txt");
                std::cout << "成功生成数独终局：" << numGrids << " 个" << std::endl;
                return 0;
            }
        }
        else if (command == "-s" && argc == 3) {
            // 处理解决数独问题的情况
            std::string puzzleFile = argv[2];
            solveSudokuPuzzles(puzzleFile);
            std::cout << "成功解决数独问题，并将结果写入文件：" << "output.txt" << std::endl;
            return 0;
        }
        else if (command == "-n" && argc >= 3) {
            int numGames = std::atoi(argv[2]);

            if (numGames >= 1 && numGames <= 10000) {
                bool uniqueSolution = false;
                int minClues = 20;
                int maxClues = 55;

                for (int i = 3; i < argc; i++) {
                    std::string option = argv[i];

                    if (option == "-u") {
                        // 处理唯一解选项
                        uniqueSolution = true;
                    }
                    else if (option == "-r" && i + 1 < argc) {
                        // 处理挖空范围选项
                        std::string range = argv[i + 1];
                        size_t delimiterPos = range.find('~');
                        if (delimiterPos != std::string::npos) {
                            std::string minRange = range.substr(0, delimiterPos);
                            std::string maxRange = range.substr(delimiterPos + 1);
                            minClues = std::atoi(minRange.c_str());
                            maxClues = std::atoi(maxRange.c_str());
                            if (minClues < 20 || maxClues > 55 || minClues > maxClues) {
                                std::cerr << "无效的挖空范围：" << range << std::endl;
                                return 0;
                            }
                            i++;  // 跳过下一个参数
                        }
                    }
                }

                generateSudokuGames(numGames, minClues, maxClues, uniqueSolution, "game.txt");
                std::cout << "成功生成数独游戏：" << numGames << " 个";
                if (uniqueSolution) {
                    std::cout << "，每个游戏有唯一解";
                }
                std::cout << "，挖空范围：" << minClues << " 到 " << maxClues << std::endl;
                return 0;
            }
        }
    }

    // 无效的命令行参数
    std::cout << "命令行参数错误，请按照以下格式运行程序：" << std::endl;
    std::cout << "生成数独终局：sudoku.exe -c 数量（数量范围：1-1000000）" << std::endl;
    std::cout << "解决数独问题：sudoku.exe -s 数独文件路径 解决方案文件路径" << std::endl;
    std::cout << "生成数独游戏：sudoku.exe -n 数量" << std::endl;
    std::cout << "生成数独游戏（挖空范围）：sudoku.exe -n 数量 -r 最小值~最大值（最小值范围：20-50）" << std::endl;
    std::cout << "生成数独游戏（唯一解）：sudoku.exe -n 数量 -u" << std::endl;

    return 0;
}


