int f(){
	write("f\n");
	return 1;
}

int g(){
	write("g\n");
	return 0;
}

int main(){
	if (f() || f()){
		write("f correct\n");
	} else {
		write("wrong\n");
	};

	if (f() && f()){
		write("f f correct\n");
	} else {
		write("wrong\n");
	};

	if (g() || g()){
		write("wrong\n");
	} else {
		write("g g correct\n");
	};

	if (g() && g()){
		write("wrong\n");
	} else {
		write("g correct\n");
	};
}
