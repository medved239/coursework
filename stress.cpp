#include <bits/stdc++.h>

using namespace std;

int main() {
    int I = 0;
    while (true) {
        ofstream out("in.in");
        vector<int> aw = {2, 3, 5, 7};

        int A = aw.size();
        out << A << endl;
        for (int i = 0; i < A; i++) out << aw[i] << " ";
        out << endl;

        int n = 20;
        for (int i = 0; i < n; i++) {
            out << (rand() % 2 ? "0" : "1");
        }
        out << endl;
        out.close();

        system("branched1_gen.exe > a.1");
        system("branched2_gen.exe > a.2");
        system("branched3_gen.exe > a.3");

        ifstream in1("a.1");
        ifstream in2("a.2");
        ifstream in3("a.3");

        for (int i = 0; i < n + 1; i++) {
            string s1, s2, s3;
            getline(in1, s1);
            getline(in2, s2);
            getline(in3, s3);
            cout << s1 << endl;
            if (s1 != s2 || s2 != s3) {
                return 0;
            }

        }

        cout << ++I << " -----------------" << endl;
    }
    return 0;
}
