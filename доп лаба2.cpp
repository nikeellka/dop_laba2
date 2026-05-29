#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Bill {
    int value;
    int count;
};

vector<Bill> wallet;
vector<int> best;
vector<int> cur;

int amount;
string strategy;

int bestDiff = 1000000;

void dfs(int pos, int sum) {

    if (sum == amount) {

        if (strategy == "UNIFORM") {

            int mn = 1000000;
            int mx = -1;

            for (int x : cur) {
                mn = min(mn, x);
                mx = max(mx, x);
            }

            if (mx - mn < bestDiff) {
                bestDiff = mx - mn;
                best = cur;
            }
        }
        else {
            best = cur;
        }

        return;
    }

    if (sum > amount || pos >= wallet.size())
        return;

    vector<int> order;

    for (int i = 0; i <= wallet[pos].count; i++) {
        order.push_back(i);
    }

    if (strategy == "MAX") {
        reverse(order.begin(), order.end());
    }

    for (int cnt : order) {

        cur[pos] = cnt;

        dfs(pos + 1, sum + cnt * wallet[pos].value);

        if ((strategy == "MAX" || strategy == "MIN")
            && !best.empty())
            return;
    }

    cur[pos] = 0;
}

int main() {

    ifstream fin("input.json");

    string s, line;

    while (getline(fin, line)) {
        s += line;
    }

    int p = s.find("\"amount\"");
    p = s.find(":", p);
    amount = stoi(s.substr(p + 1));

    p = s.find("\"strategy\"");
    p = s.find("\"", p + 11);
    int p2 = s.find("\"", p + 1);
    strategy = s.substr(p + 1, p2 - p - 1);

    p = s.find("[[");

    while (true) {

        int l = s.find("[", p);
        int r = s.find("]", l);

        if (l == string::npos || r == string::npos)
            break;

        int a, b;

        if (sscanf(s.substr(l + 1, r - l - 1).c_str(),
            "%d,%d", &a, &b) == 2) {

            wallet.push_back({ a, b });
        }

        p = r + 1;

        if (s[p] == ']')
            break;
    }

    if (strategy == "MAX") {

        sort(wallet.begin(), wallet.end(),
            [](Bill a, Bill b) {
                return a.value > b.value;
            });

    }
    else {

        sort(wallet.begin(), wallet.end(),
            [](Bill a, Bill b) {
                return a.value < b.value;
            });
    }

    cur.resize(wallet.size(), 0);

    dfs(0, 0);

    ofstream fout("output.json");

    fout << "[\n{\n\"dispense\": [";

    bool first = true;

    for (int i = 0; i < best.size(); i++) {

        if (best[i] > 0) {

            if (!first)
                fout << ", ";

            fout << "[" << wallet[i].value
                << ", " << best[i] << "]";

            first = false;
        }
    }

    fout << "]\n}\n]";

    return 0;
}