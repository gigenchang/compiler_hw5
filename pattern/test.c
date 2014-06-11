
float bbb()
{
	return 4.0 + (3.0 + (2.0 + fread()));
}

int main() {
	int a;
	a = 0;
	while(a < 5){
		int b = 0;
		while(b < 10){
			write(a);
			write("\t");
			write(b);
			write("\n");
			b = b + 1;
		}
		a = a + 1;
	}
}
