#include <bits/stdc++.h>
using namespace std;

int fib_rec(int n) {
  if (n <= 1) {
    return 1;
  } else {
    return fib_rec(n - 1) + fib_rec(n - 2);
  }
}

int fib_it(int n) {
  int a = 0, b = 1;
  for (int i = 1; i <= n; i++) {
    int temp = a + b;
    a = b;
    b = temp;
  }
  return b;
}

int fat_rec(int n) {
  if (n == 0 || n == 1) {
    return 1;
  } else {
    return fat_rec(n - 1) * n;
  }
}

int fat_it(int n) {
  if (n <= 1) {
      return 1;
  }
  int result = 1;
  for (int i = 1; i <= n; i++) {
    result *= i;
  }
  return result;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    cout << "Usage: ./recorrencias <function> <n>" << endl;
    return 1;
  }
  string function = argv[1];
  int n = atoi(argv[2]);

  if (function == "fib_rec") {
    cout << fib_rec(n) << endl;
  }
  if (function == "fib_it") {
    cout << fib_it(n) << endl;
  }
  if (function == "fat_rec") {
    cout << fat_rec(n) << endl;
  }
  if (function == "fat_it") {
    cout << fat_it(n) << endl;
  }
  return 0;
}



/*
[] ~/repos/AED2/atv <main> ✗ time ./recorrencias fib_rec 30
1346269
./recorrencias fib_rec 30  0,03s user 0,00s system 96% cpu 0,036 total
[] ~/repos/AED2/atv <main> ✗ time ./recorrencias fib_it 30
1346269
./recorrencias fib_it 30  0,00s user 0,00s system 87% cpu 0,008 total
[] ~/repos/AED2/atv <main> ✗ time ./recorrencias fat_rec 30
1409286144
./recorrencias fat_rec 30  0,00s user 0,00s system 87% cpu 0,006 total
[] ~/repos/AED2/atv <main> ✗ time ./recorrencias fat_it 30
1409286144
./recorrencias fat_it 30  0,00s user 0,00s system 86% cpu 0,003 total
[] ~/repos/AED2/atv <main> ✗


Dell Latitude 3410, I5 10210u, 16gb ram, archlinux
*/
