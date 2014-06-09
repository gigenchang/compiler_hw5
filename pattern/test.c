int main() {
	int a[2][3];
	int b;
	a[0][0] = 1;
	a[0][1] = 2;
	a[0][2] = 3;
	a[1][0] = 4;
	a[1][1] = 5;
	a[1][2] = 6;
	b = a[1][1]*a[1][2];
	write(a[0][0]);
	write("\n");
	write(a[0][1]);
	write("\n");
	write(a[0][2]);
	write("\n");
	write(a[1][0]);
	write("\n");
	write(a[1][1]);
	write("\n");
	write(b);
	write("\n");
}
