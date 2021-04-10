#include <bits/stdc++.h>

using namespace std;

vector<int> aw; // массы аминокислот

const int maxW = 100;
const int maxS = 100;

int dp0[maxW][maxW][maxS];
int dp[maxW][maxW][maxS];
int ans[maxW][maxS];
int dp_sym[maxW][maxS];
int total_sym[maxW][maxS];

// initv -- начальные значени€. ѕо умолчанию dp(0, 0) = 1, остальные нули
vector<vector<int>> linear(vector<int> spec, int S, const vector<int>& initv = {}) {
    int maxA = aw.size();
    int W = spec.size();

    auto dp = vector<vector<int>>(W, vector<int>(S, 0));
    if (initv.size() == 0) dp[0][0] = 1;
    else {
        for (int s = 0; s < S; s++) dp[0][s] = initv[s];
    }

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

vector<int> shift_spec(int d, vector<int> spec) {
    int n = spec.size();
    vector<int> res(n - d);
    for (int i = d; i < n; i++) {
        res[i - d] = spec[i];
    }
    return res;
}

// dp0(w1, w2, s) -- число пептидов массы w2, у которых есть префикс массы ровно w1, со счЄтом s
void build_dp0(vector<int> spec, int S) {
    int W = spec.size();

    auto dp_lin1 = linear(spec, S);
    for (int w = 0; w < W; w++) {
        auto spec1 = shift_spec(w, spec);

        vector<int> initv(S);
        for (int s = 0; s < S; s++) initv[s] = dp_lin1[w][s];

        auto dp_lin2 = linear(spec1, S, initv);

        for (int dw = 0; dw < W - w; dw++) {
            for (int s = 0; s < S; s++) {
                dp0[w][w + dw][s] = dp_lin2[dw][s];
            }
        }
    }
}

// dp -- см. статью
// O(W^2 * S * maxA)
void solve(vector<int> spec, int S) {
    int maxA = aw.size();
    int W = spec.size();

    build_dp0(spec, S);
    for (int w1 = 0; w1 < W; w1++) {
        for (int w2 = 1; w2 < W; w2++) {
            for (int s = 0; s < S; s++) {
                for (int a = 0; a < maxA; a++) {
                    if (w1 < aw[a]) continue;
                    if (s < spec[w1]) continue;

                    dp[w1][w2][s] += dp[w1 - aw[a]][w2][s - spec[w1]] + dp0[w1 - aw[a]][w1 - aw[a] + w2][s - spec[w1]];
                }
            }
        }
    }

    for (int w1 = 0; w1 < W; w1++) {
        for (int w2 = 1; w2 < W - w1; w2++) {
            for (int s = 0; s < S; s++) {
                ans[w1 + w2][s] += dp[w1][w2][s];
            }
        }
    }

    // теперь нужно убрать повторы
    auto dp_lin = linear(spec, S);
    for (int wp = 0; wp < W; wp++) {
        for (int w = 0; wp + 2 * w < W; w++)
            for (int s = 0; s < S; s++)
                dp_sym[w][s] = 0;
        for (int s = 0; s < S; s++)
            dp_sym[0][s] = dp_lin[wp][s];

        for (int w = 0; wp + 2 * w < W; w++) {
            for (int s = 0; s < S; s++) {
                for (int a = 0; a < maxA; a++) {
                    if (w < aw[a]) continue;
                    if (s < 2 * spec[wp + w]) continue;

                    dp_sym[w][s] += dp_sym[w - aw[a]][s - 2 * spec[wp + w]];
                }
            }
        }

        for (int w = 1; wp + 2 * w < W; w++) {
            for (int s = 0; s < S; s++) {
                total_sym[wp + 2 * w][s] += dp_sym[w][s];
            }
        }
    }
    for (int w = 0; w < W; w++) {
        for (int s = 0; s < S; s++) {
            int sym = total_sym[w][s];
            ans[w][s] -= sym;
            assert(ans[w][s] % 2 == 0); ans[w][s] /= 2;
            ans[w][s] += sym;
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
