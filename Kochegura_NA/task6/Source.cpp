#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;
void drawShipArt() {
    cout << "\n";
    cout << "                    /\\\n";
    cout << "                   /  \\\n";
    cout << "                  /    \\\n";
    cout << "                 /      \\\n";
    cout << "                /        \\\n";
    cout << "               /__________\\\n";
    cout << "               |  OCEAN   |\n";
    cout << "               |  BATTLE  |\n";
    cout << "               |__________|\n";
    cout << "               |  [][][]  |\n";
    cout << "               |__________|\n";
    cout << "                    ||\n";
    cout << "                    ||\n";
    cout << "                   (||)\n";
    cout << "                   (||)\n";
    cout << "                    ()\n";
    cout << "\n";
}
void showMainMenu() {
    cout << "========================================\n";
    cout << "              Sea fight\n";
    cout << "========================================\n";
    drawShipArt();
    cout << "1. Start\n";
    cout << "2. Exit\n";
    cout << "----------------------------------------\n";
    cout << "Your choise: ";
}

enum CellState {
    EMPTY,
    SHIP,
    HIT,
    MISS
};

class GameField {
private:
    CellState grid[10][10];

public:
    GameField() {
        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 10; j++)
                grid[i][j] = EMPTY;
    }

    bool canPlaceShip(int row, int col, int len, int direction) {
        if (direction == 0) { 
            if (col + len > 10) return false;
            for (int i = -1; i <= len; i++) {
                for (int dr = -1; dr <= 1; dr++) {
                    int r = row + dr;
                    int c = col + i;
                    if (r >= 0 && r < 10 && c >= 0 && c < 10) {
                        if (i >= 0 && i < len && dr == 0) {
                            if (grid[r][c] == SHIP) return false;
                        }
                        else {
                            if (grid[r][c] == SHIP) return false;
                        }
                    }
                }
            }
        }
        else { 
            if (row + len > 10) return false;
            for (int i = -1; i <= len; i++) {
                for (int dc = -1; dc <= 1; dc++) {
                    int r = row + i;
                    int c = col + dc;
                    if (r >= 0 && r < 10 && c >= 0 && c < 10) {
                        if (i >= 0 && i < len && dc == 0) {
                            if (grid[r][c] == SHIP) return false;
                        }
                        else {
                            if (grid[r][c] == SHIP) return false;
                        }
                    }
                }
            }
        }
        return true;
    }

    void placeShip(int row, int col, int len, int direction) {
        if (direction == 0) {
            for (int i = 0; i < len; i++)
                grid[row][col + i] = SHIP;
        }
        else {
            for (int i = 0; i < len; i++)
                grid[row + i][col] = SHIP;
        }
    }

    bool shoot(int row, int col) {
        if (row < 0 || row >= 10 || col < 0 || col >= 10) return false;
        if (grid[row][col] == SHIP) {
            grid[row][col] = HIT;
            return true;
        }
        else if (grid[row][col] == EMPTY) {
            grid[row][col] = MISS;
            return false;
        }
        return false;
    }

    bool allShipsSunk() {
        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 10; j++)
                if (grid[i][j] == SHIP)
                    return false;
        return true;
    }

    void display(bool showShips) {
        cout << "  1 2 3 4 5 6 7 8 9 10\n";
        for (int i = 0; i < 10; i++) {
            cout << char('A' + i) << " ";
            for (int j = 0; j < 10; j++) {
                if (grid[i][j] == SHIP && showShips)
                    cout << "# ";
                else if (grid[i][j] == HIT)
                    cout << "X ";
                else if (grid[i][j] == MISS)
                    cout << "O ";
                else
                    cout << ". ";
            }
            cout << endl;
        }
    }

    CellState getState(int row, int col) {
        return grid[row][col];
    }

    void clear() {
        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 10; j++)
                grid[i][j] = EMPTY;
    }
};

class Player {
protected:
    GameField myField;
    GameField enemyField;
    string name;
public:
    Player(string n) : name(n) {}
    virtual ~Player() {}

    virtual void placeShips() = 0;
    virtual pair<int, int> makeMove() = 0;

    GameField& getMyField() { return myField; }
    GameField& getEnemyField() { return enemyField; }

    bool shootAt(Player& opponent, int row, int col) {
        bool hit = opponent.getMyField().shoot(row, col);
        enemyField.shoot(row, col);
        return hit;
    }

    bool hasWonAgainst(Player& opponent) {
        return opponent.getMyField().allShipsSunk();
    }

    string getName() { return name; }
};

class ComputerPlayer : public Player {
private:
    bool used[10][10];
public:
    ComputerPlayer(string n) : Player(n) {
        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 10; j++)
                used[i][j] = false;
    }

    void placeShips() override {
        int ships[] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

        for (int len : ships) {
            bool placed = false;
            while (!placed) {
                int row = rand() % 10;
                int col = rand() % 10;
                int dir = rand() % 2;
                if (myField.canPlaceShip(row, col, len, dir)) {
                    myField.placeShip(row, col, len, dir);
                    placed = true;
                }
            }
        }
    }

    pair<int, int> makeMove() override {
        vector<pair<int, int>> available;
        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 10; j++)
                if (!used[i][j])
                    available.push_back({ i, j });

        int idx = rand() % available.size();
        int row = available[idx].first;
        int col = available[idx].second;
        used[row][col] = true;
        return { row, col };
    }
};

class HumanPlayer : public Player {
public:
    HumanPlayer(string n) : Player(n) {}

    void placeShips() override {
        int ships[] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

        for (int len : ships) {
            bool placed = false;
            while (!placed) {

                placed = true; 
            }
        }
    }

    bool tryPlaceShip(int len, int row, int col, int direction) {
        if (myField.canPlaceShip(row, col, len, direction)) {
            myField.placeShip(row, col, len, direction);
            return true;
        }
        return false;
    }

    bool isValidMove(int row, int col) {
        if (row < 0 || row >= 10 || col < 0 || col >= 10) return false;
        CellState state = enemyField.getState(row, col);
        return (state != HIT && state != MISS);
    }

    pair<int, int> makeMove() override {
        return { -1, -1 };
    }

    bool makeHumanMove(int row, int col, ComputerPlayer& computer) {
        if (!isValidMove(row, col)) return false;
        return shootAt(computer, row, col);
    }
};

class BattleshipGame {
private:
    HumanPlayer human;
    ComputerPlayer computer;
    bool humanTurn;

public:
    BattleshipGame() : human("Игрок"), computer("Компьютер") {
        srand(time(0));
        humanTurn = true;
    }

    void run() {
        cout << "=== SEA FIGHT ===\n";

        cout << "\n=== PLACE SHIPS ===\n";

        int ships[] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

        for (int len : ships) {
            human.getMyField().display(true);
            bool placed = false;
            while (!placed) {
                cout << "\nship len " << len << ". Enter: ";
                char rowChar, orient;
                int col;
                cin >> rowChar >> col >> orient;

                int row = toupper(rowChar) - 'A';
                col--;
                orient = toupper(orient);

                if (row < 0 || row >= 10 || col < 0 || col >= 10) {
                    cout << "Error\n";
                    continue;
                }
                int dir = (orient == 'H') ? 0 : 1;
                if (human.tryPlaceShip(len, row, col, dir)) {
                    placed = true;
                }
                else {
                    cout << "Cant place here\n";
                }
            }
        }
        cout << "\nEnd placement!\n";
        human.getMyField().display(true);

        computer.placeShips();

        cout << "\n=== Start ===\n";
        cout << "First player\n";

        while (true) {
            cout << "\n----------------------------------------\n";
            if (humanTurn) {
                cout << "Player make\n";
                cout << "\nYour shoots:\n";
                human.getEnemyField().display(false);
                bool validMove = false;
                int row, col;
                while (!validMove) {
                    cout << "\nenter coordinates: ";
                    char rowChar;
                    cin >> rowChar >> col;
                    row = toupper(rowChar) - 'A';
                    col--;
                    if (row < 0 || row >= 10 || col < 0 || col >= 10) {
                        cout << "Error\n";
                        continue;
                    }
                    if (!human.isValidMove(row, col)) {
                        cout << "Cant shoot 2 times at 1 place\n";
                        continue;
                    }
                    validMove = true;
                }
                bool hit = human.shootAt(computer, row, col);
                if (hit) {
                    cout << "Yeeeees, got him!\n";
                    human.getEnemyField().display(false);
                }
                else {
                    cout << "Nooooooo, sad!\n";
                }
                if (human.hasWonAgainst(computer)) {
                    cout << "\n========================================\n";
                    cout << "You win!\n";
                    cout << "========================================\n";
                    break;
                }
                if (!hit) {
                    humanTurn = false;
                }
            }
            else {
                cout << "Computer make shoot\n";
                pair<int, int> move = computer.makeMove();
                cout << "shoot on " << char('A' + move.first) << move.second + 1 << endl;
                bool hit = computer.shootAt(human, move.first, move.second);
                if (hit) {
                    cout << "Oh no, he is right!\n";
                }
                else {
                    cout << "Yeeees, he lose this shoot!\n";
                }
                if (computer.hasWonAgainst(human)) {
                    cout << "\n========================================\n";
                    cout << "Computer win!\n";
                    cout << "========================================\n";
                    break;
                }
                if (!hit) {
                    humanTurn = true;
                }
            }
        }
    }
};
int main() {
    int choice;
    do {
        showMainMenu();
        cin >> choice;
        switch (choice) {
        case 1:
        {
            cin.ignore(); 
            BattleshipGame game;
            game.run();
        }
        break;
        break;
        case 2:
            cout << "\nThanks for game!\n";
            break;
        default:
            cout << "\nWrong choise\n";
            break;
        }
        if (choice != 2) {
            cout << "\nPress Enter...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    } while (choice != 2);
    return 0;
}