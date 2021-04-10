#include <bits/stdc++.h>

using namespace std;

vector<int> aw; // массы аминокислот

const int maxW = 100;
const int maxS = 100;

int dp_nopr[maxW][maxW][maxS];
int ans[maxW][maxS];

vector<vector<int>> linear(vector<int> spec, int S) {
    int maxA = aw.size();
    int W = spec.size();

    auto dp = vector<vector<int>>(W, vector<int>(S, 0));
    dp[0][0] = 1;

    for (int w = 1; w < W; w++) {
        for (int s = 0; s < S; s++) {
            for (int a = 0; a < maxA; a++) {
                if (w < aw[a]) continue;
                if (s < spec[w]) continue;

                dp[w][s] += dp[w - aw[a]][s - spec[w]];
            }
        }
    }

    return dp;
}

void solve_noprefix(vector<int> spec, int S) {
    int maxA = aw.size();
    int W = spec.size();

    for (int w1 = 0; w1 < W; w1++)
        for (int w2 = 0; w2 < W; w2++)
            for (int s = 0; s < S; s++)
                if (w1 + w2 > 0) dp_nopr[w1][w2][s] = 0; // начальные данные не чистим

    // сперва считаем dp_nopr(w1, 0, s) аналогично линейному случаю
    for (int w1 = 1; w1 < W; w1++) {
        for (int s = 0; s < S; s++) {
            for (int a = 0; a < maxA; a++) {
                if (w1 < aw[a]) continue;
                if (s < spec[w1]) continue;

                dp_nopr[w1][0][s] += dp_nopr[w1 - aw[a]][0][s - spec[w1]];
            }
        }
    }

    // теперь для каждого фиксированного w1 досчитываем ненулевые w2 аналогично линейному случаю, но с начальными данными из предыдущего шага
    for (int w1 = 0; w1 < W; w1++) {
        for (int w2 = 1; w2 < W; w2++) {
            for (int s = 0; s < S; s++) {
                for (int a = 0; a < maxA; a++) {
                    if (w2 < aw[a]) continue;
                    if (s < spec[w2]) continue;

                    dp_nopr[w1][w2][s] += dp_nopr[w1][w2 - aw[a]][s - spec[w2]];
                }
            }
        }
    }

    // считаем случай совпадающих ветвей
    vector<vector<int>> dp_symm(W, vector<int>(S, 0));
    for (int s = 0; s < S; s++) {
        dp_symm[0][s] = dp_nopr[0][0][s];
    }
    for (int w = 1; w < W; w++) {
        for (int s = 0; s < S; s++) {
            for (int a = 0; a < maxA; a++) {
                if (w < aw[a]) continue;
                if (s < 2 * spec[w]) continue;

                dp_symm[w][s] += dp_symm[w - aw[a]][s - 2 * spec[w]];
            }
        }
    }

    // убираем дважды посчитанные пептиды (с одинаковой длиной ветвей)
    for (int w = 0; w < W; w++) {
        for (int s = 0; s < S; s++) {
            int symm = dp_symm[w][s];

            dp_nopr[w][w][s] -= symm;
            assert(dp_nopr[w][w][s] % 2 == 0); dp_nopr[w][w][s] /= 2;
            dp_nopr[w][w][s] += symm;
        }
    }
}

vector<int> shift_spec(int d, vector<int> spec) {
    int n = spec.size();
    vector<int> res(n - d);
    for (int i = d; i < n; i++) {
        res[i - d] = spec[i];
    }
    return res;
}

// O(W^3 * S * maxA)
void solve(vector<int> spec, int S) {
    int W = spec.size();

    auto dp_lin = linear(spec, S);

    for (int w = 0; w < W; w++) {
        auto spec1 = shift_spec(w, spec);

        // задаем начальные данные
        for (int s = 0; s < S; s++) {
            dp_nopr[0][0][s] = dp_lin[w][s];
        }
        solve_noprefix(spec1, S);

        for (int w1 = 1; w1 < W - w; w1++) {
            for (int w2 = 1; w2 < W - w; w2++) {
                if (w + w1 + w2 >= W) continue;
                if (w1 > w2) continue; // не считаем по два раза пептиды с разными длинами ветвей
                for (int s = 0; s < S; s++) {
                    ans[w + w1 + w2][s] += dp_nopr[w1][w2][s];
                }
            }
        }
    }
}

int main() {
    freopen("in.in", "r", stdin);
    //freopen("in2.out", "w", stdout);

    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int A;
    cin >> A;
    aw.resize(A);
    for (int i = 0; i < A; i++) cin >> aw[i];

    string str;
    cin >> str;
    vector<int> spec(str.size() + 1);
    for (int i = 0; i < str.size(); i++) {
        spec[i + 1] = (str[i] == '1');
    }

    int W = spec.size();
    int S = W / 2 + 1;
    solve(spec, S);

    for (int w = 0; w < W; w++) {
        cout << w << ": ";
        for (int s = 0; s < S; s++) {
            cout << ans[w][s] << " ";
        }
        cout << endl;
    }

    return 0;
}
