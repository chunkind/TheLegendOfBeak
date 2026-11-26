#include <iostream>

using namespace std;

int a, b, c;
int p[11];

int pow(int a, int b)
{
	int r = a;
	for (int i = 1; i <= b; i++) {
		if (i > 10)
			break;
		r = (r * a) % c;
		p[i] = r;
	}
	return p[b%c];
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> a >> b >> c;

	cout << pow(a, b) % c << '\n';

	return 0;
}