#include <bits/stdc++.h>

using namespace std;

vector<int> aw; // массы аминокислот

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

vector<vector<int>> solve_symm(vector<int> spec, int S) {
    int W = spec.size();

    auto dp = linear(spec, S / 2 + 1);
    auto res = vector<vector<int>>(W, vector<int>(S, 0));
    for (int w = 0; w < W; w++) {
        for (int s = 0; s < S; s += 2) {
            res[w][s] = dp[w][s / 2];
        }
    }
    return res;
}

const int maxW = 100;
const int maxS = 100;

int dp_nopr[maxW][maxS];
int ans[maxW][maxS];

void solve_noprefix(vector<int> spec, int S) {
    int W = spec.size();
    auto dp_lin = linear(spec, S);

    for (int w = 0; w < W; w++) {
        for (int s = 0; s < S; s++) {
            dp_nopr[w][s] = 0;
        }
    }

    // тут должно быть 2D-FFT
    for (int w = 0; w < W; w++) {
        for (int s = 0; s < S; s++) {
            for (int w1 = 1; w1 <= w - 1; w1++) {
                for (int s1 = 0; s1 <= s; s1++) {
                    dp_nopr[w][s] += dp_lin[w1][s1] * dp_lin[w - w1][s - s1];
                }
            }
        }
    }

    // убираем дважды посчитанные пептиды
    auto dp_s = solve_symm(spec, S);

    for (int w = 1; w < W; w++) {
        for (int s = 0; s < S; s++) {
            int symm = 0;
            if (w % 2 == 0) symm = dp_s[w / 2][s];
            assert((dp_nopr[w][s] - symm) % 2 == 0);
            dp_nopr[w][s] = (dp_nopr[w][s] - symm) / 2 + symm;
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

// O(W^2 * S * log(WS)), åñëè ñ FFT
void solve(vector<int> spec, int S) {
    int W = spec.size();
    auto dp_lin = linear(spec, S);

    for (int w = 0; w < W; w++) {
        auto spec1 = shift_spec(w, spec);
        solve_noprefix(spec1, S);

        // тут должно быть FFT
        for (int w1 = 0; w1 < W - w; w1++) {
            for (int s = 0; s < S; s++) {
                for (int s1 = 0; s1 < S - s; s1++) {
                    ans[w + w1][s + s1] += dp_lin[w][s] * dp_nopr[w1][s1];
                }
            }
        }
    }
}


int main() {
    freopen("in.in", "r", stdin);
    //freopen("in3.out", "w", stdout);

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
