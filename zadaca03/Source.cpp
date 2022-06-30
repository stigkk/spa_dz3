#include<iostream>
#include <cmath>
#include <list>
#include <vector>
#include <algorithm>

using namespace std;

class pozic
{
    int x, y;
public:
    pozic(int _x, int _y) : x(_x), y(_y) {}
    pozic() = default;
    pozic operator +(const pozic& ost) {
        pozic temp;
        temp.x = this->x + ost.x;
        temp.y = this->y + ost.y;
        return temp;
    }
    int getX() const { return x; }
    int getY() const { return y; }

    friend class Map;
};

struct Node
{
    pozic pozicija;
    int G, H, F;
    Node* parent = nullptr;

    Node() = default;
    Node(const Node& other) = default;
    Node(pozic pos) :pozicija(pos) {};

    void calc(const pozic& endPos) {
        H = static_cast<int>((abs(static_cast<double>(pozicija.getX() - endPos.getX())) + abs(static_cast<double>(pozicija.getY() - endPos.getY()))));
        G = parent ? parent->G + 1 : 1;
        F = G + H;
    }

    bool operator==(const Node& ost) const {
        return (pozicija.getX() == ost.pozicija.getX() && pozicija.getY() == ost.pozicija.getY());
    }
    bool operator!=(const Node& ost) const {
        return !(*this == ost);
    }
    bool operator<(const Node& ost)  const {
        return(F < ost.F);
    }
};

class Map
{
    std::vector<char> data;
    int size;
public:
    Map() = default;
    Map(int _size) : size(_size) {
        data.resize(size * size);
        for (int i = 0; i < size * size; ++i) data[i] = '.';
    }
    void display() const {
        for (int i = 1; i <= size * size; ++i) {
            std::cout << data[i - 1] << " ";
            if (!(i % size)) std::cout << "\n";
        }
    }
    bool getIfInDanger(pozic pozicija) const {
        if (pozicija.y < 0) pozicija.y = 0;
        if (pozicija.x < 0) pozicija.x = 0;
        if (pozicija.y >= 20) pozicija.y = size - 1;
        if (pozicija.x >= 20) pozicija.x = size - 1;
        return(data[pozicija.getX() + (pozicija.getY() * size)] == 'X');
    }
    void setElement(char&& asda, pozic pozicija) {
        data[pozicija.getX() + (pozicija.getY() * size)] = asda;
    }
};

class Solver
{
    pozic startPos, endPos;
    vector<pozic> directions;
    Map map;
public:
    Solver(pozic _startPos, pozic _endPos, int size) : startPos(_startPos), endPos(_endPos) {
        Map temp(size);
        map = temp;

        map.setElement('X', pozic(14, 15));
        map.setElement('X', pozic(15, 15));
        map.setElement('X', pozic(16, 15));
        map.setElement('X', pozic(16, 14));
        map.setElement('X', pozic(16, 13));

        directions.resize(8);
        directions[0] = pozic(-1, 1);
        directions[1] = pozic(-1, 0);
        directions[2] = pozic(-1, -1);
        directions[3] = pozic(0, 1);
        directions[4] = pozic(0, -1);
        directions[5] = pozic(1, 1);
        directions[6] = pozic(1, 0);
        directions[7] = pozic(1, -1);
    }
    bool aStar() {
        Node startNode(startPos);
        Node goalNode(pozic(endPos.getX(), endPos.getY()));

        if (map.getIfInDanger(startNode.pozicija) || map.getIfInDanger(goalNode.pozicija)) {
            cout << "ne moze zbog prepreke.";
            return false;
        }

        list<Node> openList;
        list<Node> closedList;

        startNode.calc(endPos);
        openList.push_back(startNode);

        while (!openList.empty()) {
            auto current = Node(*std::min_element(openList.begin(), openList.end()));

            current.calc(endPos);

            closedList.push_back(current);
            openList.remove(current);
            if (current == goalNode) break;

            for (auto& direction : directions) {
                Node successor(direction + current.pozicija);

                if (map.getIfInDanger(successor.pozicija) || successor.pozicija.getX() > 20 - 1 ||
                    successor.pozicija.getY() > 20 - 1 || successor.pozicija.getX() < 0 ||
                    successor.pozicija.getY() < 0 ||
                    find(closedList.begin(), closedList.end(), successor) != closedList.end()) {
                    continue;
                }

                successor.calc(endPos);

                auto inOpen = std::find(openList.begin(), openList.end(), successor);
                if (inOpen == openList.end()) {
                    successor.parent = &closedList.back();
                    successor.calc(endPos);

                    openList.push_back(successor);
                }
                else
                    if (successor.G < inOpen->G) successor.parent = &closedList.back();
            }
        }

        if (!openList.size()) {
            cout << "Ne mogu nac put.\n";
            return false;
        }

        auto inClosed = std::find(closedList.begin(), closedList.end(), goalNode);
        if (inClosed != closedList.end()) {
            while (*inClosed != startNode) {
                map.setElement('Y', inClosed->pozicija);
                *inClosed = *inClosed->parent;
            }
        }

        map.display();
        return true;
    }
};

int main() {

	int AA, AB, BA, BB;

	cout << "Unesite retke A: ";
	cin >> AA;

	cout << "Unesite stupac A: ";
	cin >> AB;

	cout << "Unesite retke B: ";
	cin >> BA;

	cout << "Unesite stupac A: ";
	cin >> BB;

    Solver radi(pozic(AA, AB), pozic(BA, BB), 20);
    radi.aStar();

	return 0;
}