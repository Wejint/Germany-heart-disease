#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>

using namespace std;

// ˮ������ �������enum������b��s��c��l����ˮ��
enum class FruitType {
    Banana,
    Strawberry,
    Cherry,
    Lemon
};

// ��  
class Card {
public:
    Card(FruitType fruit, int count) : fruit(fruit), count(count) {}
    FruitType getFruit() const { return fruit; }
    int getCount() const { return count; }
private:
    FruitType fruit;
    int count;
};

// ���
class Player {
public:
    Player() : score(0) {}
    void addCard(const Card& card) { cards.push_back(card); }
    void addScore(int points) { score += points; }
    int getScore() const { return score; }
    bool hasCards() const { return !cards.empty(); }
    void printCards() const {
        cout << "Player's cards: ";
        for (const auto& card : cards) {
            cout << "(" << static_cast<int>(card.getFruit()) << ", " << card.getCount() << ") ";
        }
        cout << endl;
    }
    bool operator<(const Player& other) const {
        return cards.size() < other.cards.size();
    }
private:
    vector<Card> cards;
    int score;
};

// ��Ϸ
class Game {
public:
    Game() {
        // ��ʼ���ƶ�
        for (int i = 0; i < 5; i++) {
            deck.push_back(Card(FruitType::Banana, 5));
        }
        for (int i = 0; i < 3; i++) {
            deck.push_back(Card(FruitType::Strawberry, 3));
            deck.push_back(Card(FruitType::Cherry, 3));
        }
        deck.push_back(Card(FruitType::Lemon, 2));
        deck.push_back(Card(FruitType::Lemon, 2));
        deck.push_back(Card(FruitType::Lemon, 1));
        // ϴ��
        srand(time(nullptr));
        random_shuffle(deck.begin(), deck.end());
        // ��ʼ�����
        for (int i = 0; i < 4; i++) {
            players.push_back(Player());
        }
        // ����
        for (int i = 0; i < 56; i++) {
            players[i % 4].addCard(deck[i]);
        }
        // ��ʼ�����ƶ�
        for (int i = 0; i < 4; i++) {
            openCards.push_back(players[i].hasCards() ? players[i].cards.back() : Card(FruitType::Banana, 0));
        }
    }
    void start() {
        while (true) {
            // ��ӡ���ƶ�
            printOpenCards();
            // ���γ���
            for (int i = 0; i < 4; i++) {
                // ����Ѿ�����
                if (!players[i].hasCards()) {
                    continue;
                }
                // ��ҳ���
                cout << "Player " << i << " turn: ";
                players[i].printCards();
                Card card = players[i].cards.back();
                players[i].cards.pop_back();
                openCards[i] = card;
                // �ж��Ƿ�����
                if (isRingBell()) {
                    cout << "Player " << i << " rings the bell!" << endl;
                    // �ж��Ƿ���ȷ
                    if (isValid()) {
                        cout << "Correct! Player " << i << " gets the cards." << endl;
                        players[i].addScore(1);
                        for (auto& card : openCards) {
                            players[i].addCard(card);
                        }
                        openCards.clear();
                        // ����ϴ��
                        random_shuffle(deck.begin(), deck.end());
                        for (auto& player : players) {
                            random_shuffle(player.cards.begin(), player.cards.end());
                        }
                        // ��ʼ�����ƶ�
                        for (int j = 0; j < 4; j++) {
                            openCards.push_back(players[j].hasCards() ? players[j].cards.back() : Card(FruitType::Banana, 0));
                        }
                    }
                    else {
                        cout << "Wrong! Player " << i << " gives cards to others." << endl;
                        for (int j = 0; j < 4; j++) {
                            if (j != i && players[j].hasCards()) {
                                players[j].addCard(deck.back());
                                deck.pop_back();
                            }
                        }
                    }
                    // ���¿�ʼ����
                    i = -1;
                    // �ȴ�һ��ʱ��
                    this_thread::sleep_for(chrono::seconds(1));
                }
            }
            // �ж��Ƿ����
            if (isGameOver()) {
                break;
            }
        }
        // ��ӡ����
        for (int i = 0; i < 4; i++) {
            cout << "Player " << i << " score: " << players[i].getScore() << endl;
        }
        // ��ӡʤ����
        auto winner = max_element(players.begin(), players.end());
        cout << "Player " << distance(players.begin(), winner) << " wins!" << endl;
    }
private:
    vector<Card> deck; // �ƶ�
    vector<Player> players; // ���
    vector<Card> openCards; // ���ƶ�
    bool isRingBell() const {
        int sum = 0;
        for (const auto& card : openCards) {
            sum += card.getCount() * static_cast<int>(card.getFruit()) + 1;
        }
        return sum % 5 == 0;
    }
    bool isValid() const {
        int sum = 0;
        for (const auto& card : openCards) {
            sum += card.getCount() * static_cast<int>(card.getFruit()) + 1;
        }
        return sum % 5 == 0 && openCards.back().getCount() == 1;
    }
    bool isGameOver() const {
        int count = 0;
        for (const auto& player : players) {
            if (!player.hasCards()) {
                count++;
            }
        }
        return count >= 3;
    }
    void printOpenCards() const {
        cout << "Open cards: ";
        for (const auto& card : openCards) {
            cout << "(" << static_cast<int>(card.getFruit()) << ", " << card.getCount() << ") ";
        }
        cout << endl;
    }
};

int main() {
    Game game;
    game.start();
    return 0;
}