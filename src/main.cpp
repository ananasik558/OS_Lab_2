#include <iostream>
#include <vector>
#include <thread>
#include <random>

using namespace std;
using matrix = vector<vector<int>>;

void PrintMatrix(vector<vector<int>> matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int Det(const matrix& M1) {
	int i, j, n = M1.size();
	if (n == 1) return M1[0][0];
	matrix M2(n - 1, vector<int>(n - 1));
	vector<int>line(n - 1);
	for (i = 1; i < n; ++i) {
		line[i - 1] = M1[0][i];
		for (j = 1; j < n; ++j)
			M2[i - 1][j - 1] = M1[i][j];
	}
	int ans = 0;
	for (i = 0; i < n; ++i) {
		ans += M1[i][0] * Det(M2) * (i % 2 == 0 ? 1 : -1);
		if (i != n - 1)
			swap(M2[i], line);
	}
	return ans;
}

int main() {
    int n;
    cout << "Enter size of square matrix" << '\n';
    cin >> n;
    int num_thread;
    cout << "Enter number of thread" << '\n';
    cin >> num_thread;

    matrix Matrix(n, vector<int> (n));
	int res;
    for (int i = 1; i <= num_thread; i++) {
        auto start = chrono::steady_clock::now();
		for (int i = 0; i < n; i++) {
        	for (int j = 0; j < n; j++) {
            	static random_device rd;
            	static mt19937 gen(rd());
            	uniform_real_distribution<double> dis(0, 100);
            	Matrix[i][j] = round(dis(gen));
        }
    }
        res = Det(Matrix);
        auto end = chrono::steady_clock::now();
        auto diff = end - start;
        cout << i << ' ' << chrono::duration <double, milli> (diff).count() << " ms" << endl;
		PrintMatrix(Matrix);
		cout << "determinant =" << res << '\n';
		cout << "========================================================" << '\n';
    }
	return 0;
}