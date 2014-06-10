int ch[2][3][4];

float bbb()
{
	return 4.0 + (3.0 + (2.0 + fread()));
}

int main() {

	float b;
	b = bbb();
	ch[1][0][2] = 7;
	write(ch[1][0][2]);
	write(b);
	write("\n");
}
