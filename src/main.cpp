#include <iostream>
#include <fstream>
#include <pthread.h>
#include <time.h>
#include <vector>
#include <cmath>
using namespace std;

using matrix = vector<vector<int>>;

int det(const matrix& M1) {
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
		ans += M1[i][0] * det(M2) * (i % 2 == 0 ? 1 : -1);
		if (i != n - 1)
			swap(M2[i], line);
	}
	return ans;
}

matrix M;
int res = 0;
int step;

struct pthread_args {
    int begin;
    int end;
};

void* Solve(void* arg) {
    struct pthread_args* args = ((struct pthread_args*)arg);
    int begin = args->begin, end = args->end;
    int n = M.size();
    if(n == 1) { 
        res = M[0][0];
        pthread_exit(0);
        return 0;
    }
    for (int i = begin; i < end; ++i) {
        vector<vector<int>>M2(n - 1, vector<int>(n - 1));
        int dj = 0;
        for(int j = 0; j < n - 1; ++j) {
            if(j == i) ++dj;
            for(int k = 0; k < n - 1; ++k) {
                M2[j][k] = M[j + dj][k + 1];
            }
        }
        res += M[i][0] * det(M2) * (i % 2 == 0 ? 1 : -1);
    }
    pthread_exit(0);
    return 0;
}

int main(int argc, char* argv[]) {
    string f_name;
    int flow = 0;
    for (int i = 0; i < static_cast <std::string> (argv[1]).size(); ++i)
        f_name += argv[1][i];
    for (int i = 0; i < static_cast <string> (argv[2]).size(); ++i)
        flow *= 10, flow += (argv[2][i] - '0');
    int start_flow = flow;
    ifstream fl(f_name);
    int n;
    fl >> n;
    flow = min(flow, n);
    M.resize(n, vector<int>(n));
    string s;
    for(auto & row : M) 
        for(auto & elem: row) 
            fl >> s, elem = stoi(s);
    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);            
    step = n / flow;
    pthread_t tid[flow];
    struct pthread_args* args_pth = (struct pthread_args*)malloc(flow * sizeof(struct pthread_args));
    for (int i = 0; i < flow; ++i) {
        args_pth[i].begin = i * step;
        args_pth[i].end = (i == flow - 1) ? n : (i + 1) * step;
    }
    for (int i = 0; i < flow; ++i)
        pthread_create(&tid[i], NULL, Solve, (void*)&args_pth[i]);
    for (int i = 0; i < flow; ++i)
        pthread_join(tid[i], NULL);

    clock_gettime(CLOCK_MONOTONIC, &stop);
    long long elapsed_time = (stop.tv_sec - start.tv_sec) * 1e12 + (stop.tv_nsec - start.tv_nsec);
    cout << start_flow << ' ' << elapsed_time << endl;
    cout << "res = " << res << endl;

    return 0;
}
