#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <random>

const int GRID_SIZE = 9;       // �������ӵ��ܴ�С
const int SUBGRID_SIZE = 3;    // ������Ĵ�С

typedef std::vector<std::vector<int>> SudokuGrid;

// Ѱ�������еĿո�ֵΪ0����λ��
bool findEmptyCell(const SudokuGrid& grid, int& row, int& col) {
    for (row = 0; row < GRID_SIZE; row++) {
        for (col = 0; col < GRID_SIZE; col++) {
            if (grid[row][col] == 0) {
                // �ҵ��ո���
                return true;
            }
        }
    }
    // û�пո���
    return false;
}

// ���������㷨ʵ��
bool isSafe(const SudokuGrid& grid, int row, int col, int num) {
    // ���ͬһ���Ƿ�����ظ�����
    for (int c = 0; c < GRID_SIZE; c++) {
        if (grid[row][c] == num) {
            return false;
        }
    }

    // ���ͬһ���Ƿ�����ظ�����
    for (int r = 0; r < GRID_SIZE; r++) {
        if (grid[r][col] == num) {
            return false;
        }
    }

    // ���ͬһ���Ź����Ƿ�����ظ�����
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (int r = startRow; r < startRow + 3; r++) {
        for (int c = startCol; c < startCol + 3; c++) {
            if (grid[r][c] == num) {
                return false;
            }
        }
    }

    // �������Ϸ�
    return true;
}

bool solveSudoku(SudokuGrid& grid) {
    int row, col;
    if (!findEmptyCell(grid, row, col)) {
        // ���и��Ӷ�������������������
        return true;
    }

    // ����������� 1 �� 9
    for (int num = 1; num <= 9; num++) {
        if (isSafe(grid, row, col, num)) {
            // ��䵱ǰ����
            grid[row][col] = num;

            // �ݹ鳢�������һ������
            if (solveSudoku(grid)) {
                return true;  // �ҵ��������
            }

            // ���ݣ�����ǰ��������Ϊ0
            grid[row][col] = 0;
        }
    }

    // �޷��ҵ����ʵ�������䣬���ݵ���һ��
    return false;
}


// ����һ������������վ�

SudokuGrid generateCompletedSudoku() {
    SudokuGrid grid(GRID_SIZE, std::vector<int>(GRID_SIZE));

    // ��ʼ������
    std::random_device rd;
    std::mt19937 gen(rd());
    // ʹ�� 1-9 �������������һ��
    std::vector<int> nums = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::shuffle(nums.begin(), nums.end(), gen);
    for (int i = 0; i < GRID_SIZE; i++) {
        grid[0][i] = nums[i];
    }

    // ͨ�����е�һ�е����֣��ݹ����ʣ�����
    solveSudoku(grid);

    return grid;
}



// �������վ�д���ļ�
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
        std::cerr << "�޷����ļ�����д��: " << filename << std::endl;
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

// ���ļ��ж�ȡ�����վ�
void solveSudokuPuzzles(const std::string& puzzleFile) {
    std::ifstream inputFile(puzzleFile);
    std::ofstream outputFile("output.txt");

    if (inputFile.is_open() && outputFile.is_open()) {
        std::string line;
        SudokuGrid grid;

        while (std::getline(inputFile, line)) {
            if (line.empty()) {
                continue; // ��������
            }

            if (line == "===") {
                if (!grid.empty()) {
                    // �����ǰ��������
                    solveSudoku(grid);

                    // ������������д������ļ�
                    writeSudokuToFile(grid, outputFile);
                    outputFile << std::endl << "===" << std::endl << std::endl;

                    // ��յ�ǰ��������
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

                // ��ӵ�ǰ�е���������
                if (!row.empty()) {
                    grid.push_back(row);
                }
            }
        }

        // �������һ����������
        if (!grid.empty()) {
            solveSudoku(grid);
            writeSudokuToFile(grid, outputFile);
        }

        inputFile.close();
        outputFile.close();
    }
    else {
        std::cerr << "�޷����ļ����ж�ȡ��д��: " << puzzleFile << std::endl;
    }
}



// ����������Ϸ��д���ļ�
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
        std::cerr << "�޷����ļ����ж�ȡ: " << "sudoku_syukyoku.txt" << std::endl;
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
                // ���Ҫ��Ψһ�������ɵ�����û��Ψһ�⣬��������������
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
        std::cerr << "�޷����ļ�����д��: " << filename << std::endl;
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
        std::cerr << "�޷����ļ�����д��: " << filename << std::endl;
    }
}

int main(int argc, char* argv[]) {
    // �����������
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // ��������в���
    if (argc > 1) {
        std::string command = argv[1];

        if (command == "-c" && argc == 3) {
            // �������������վֵ����
            int numGrids = std::atoi(argv[2]);
            if (numGrids >= 1 && numGrids <= 1000000) {
                generateSudokuGrids(numGrids, "sudoku_syukyoku.txt");
                std::cout << "�ɹ����������վ֣�" << numGrids << " ��" << std::endl;
                return 0;
            }
        }
        else if (command == "-s" && argc == 3) {
            // ������������������
            std::string puzzleFile = argv[2];
            solveSudokuPuzzles(puzzleFile);
            std::cout << "�ɹ�����������⣬�������д���ļ���" << "output.txt" << std::endl;
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
                        // ����Ψһ��ѡ��
                        uniqueSolution = true;
                    }
                    else if (option == "-r" && i + 1 < argc) {
                        // �����ڿշ�Χѡ��
                        std::string range = argv[i + 1];
                        size_t delimiterPos = range.find('~');
                        if (delimiterPos != std::string::npos) {
                            std::string minRange = range.substr(0, delimiterPos);
                            std::string maxRange = range.substr(delimiterPos + 1);
                            minClues = std::atoi(minRange.c_str());
                            maxClues = std::atoi(maxRange.c_str());
                            if (minClues < 20 || maxClues > 55 || minClues > maxClues) {
                                std::cerr << "��Ч���ڿշ�Χ��" << range << std::endl;
                                return 0;
                            }
                            i++;  // ������һ������
                        }
                    }
                }

                generateSudokuGames(numGames, minClues, maxClues, uniqueSolution, "game.txt");
                std::cout << "�ɹ�����������Ϸ��" << numGames << " ��";
                if (uniqueSolution) {
                    std::cout << "��ÿ����Ϸ��Ψһ��";
                }
                std::cout << "���ڿշ�Χ��" << minClues << " �� " << maxClues << std::endl;
                return 0;
            }
        }
    }

    // ��Ч�������в���
    std::cout << "�����в��������밴�����¸�ʽ���г���" << std::endl;
    std::cout << "���������վ֣�sudoku.exe -c ������������Χ��1-1000000��" << std::endl;
    std::cout << "����������⣺sudoku.exe -s �����ļ�·�� ��������ļ�·��" << std::endl;
    std::cout << "����������Ϸ��sudoku.exe -n ����" << std::endl;
    std::cout << "����������Ϸ���ڿշ�Χ����sudoku.exe -n ���� -r ��Сֵ~���ֵ����Сֵ��Χ��20-50��" << std::endl;
    std::cout << "����������Ϸ��Ψһ�⣩��sudoku.exe -n ���� -u" << std::endl;

    return 0;
}


