#include "std_lib_facilities.hpp" 
string permutacja(string wyraz, int *index, int n); 
void execute();
string zmien_na_bity(string wyraz, int n);
string Xor(string wyraz, string klucz, int n);
string sboks(string half, int box[][4], int check[][2]); 
string runda(string wyraz, string klucz);



struct indexy
{
	int PWw[8] = { 1,5,2,0,3,7,4,6 }; // permutacja wstepna wyrazu
	int PWk[10] = { 2,4,1,6,3,9,0,8,7,5 }; // permutacja wstepna klucza
	int SL1[5] = { 1,2,3,4,0 }; //przesuniecie w lewo
	int SL2[5] = { 2,3,4,0,1 }; //przesuniecie dwa razy w lewo
	int P10w8[8] = { 5,2,6,3,7,4,9,8 }; //zrobienie 8-bitowego ciagu znakow z 10-bitowego
	int P4[4] = { 1,3,2,0 }; //zrobienie 4-bitowego ciagu znakow
	int P4w8[8] = { 3,0,1,2,1,2,3,0 }; //zrobienie 8-bitowego ciagu znakow z 4-bitowego
	int cross[8] = { 4,5,6,7,0,1,2,3 }; //krzyzowanie, czyli zamiana pierwszych 4 bitow z ostatnimi
	int PO[8] = { 3,0,2,4,6,1,7,5 }; //permutacja odwrotna
};

struct SBoxy
{
	int SBox1[4][4] = { { 1,0,3,2 },{ 3,2,1,0 },{ 0,2,1,3 },{ 3,1,3,2 } };
	int SBox2[4][4] = { { 0,1,2,3 },{ 2,0,1,3 },{ 3,0,1,0 },{ 2,1,0,3 } };
	int check[2][2] = { { 0,1 },{ 2,3 } };
};

int main()
{

	indexy index;
	string wyraz;
	string klucz;
	cout << "Podaj ciag znakow do zaszyfrowania ";
	cin >> setw(8) >> wyraz;
	cout << "Podaj klucz ";
	cin >> setw(10) >> klucz;
	wyraz = zmien_na_bity(wyraz, 8);
	klucz = zmien_na_bity(klucz, 10);


	string PWk = permutacja(klucz, index.PWk, 10);
	string k0 = "", k1 = "";
	for (int i = 0; i < 10; i++)
	{
		if (i > 4) k1 += PWk[i];
		else k0 += PWk[i];
	}
	k0 = permutacja(k0, index.SL1, 5);
	k1 = permutacja(k1, index.SL1, 5);
	string klucz1 = permutacja(k0 + k1, index.P10w8, 8);
	k0 = permutacja(k0, index.SL2, 5);
	k1 = permutacja(k1, index.SL2, 5);
	string klucz2 = permutacja(k0 + k1, index.P10w8, 8);
	cout << "klucz nr 1: " << klucz1 << endl << "klucz nr 2: " << klucz2 << endl;

	string PWw = Permutacja(wyraz, index.PWw, 8);
	PWw = runda(PWw, klucz1);
	cout << "Wynik szyfrowania kluczem pierwszej rundy: " << PWw << endl;
	PWw = Permutacja(PWw, index.cross, 8);
	PWw = runda(PWw, klucz2);
	cout << "Wynik szyfrowania kluczem drugiej rundy: " << PWw << endl;
	PWw = Permutacja(PWw, index.PO, 8);
	cout << "Zaszyfrowana wiadomosc: " << PWw << endl;

	//Tutaj sie zaczyna odszyfrowywanie
	cout << "Podaj szyfrogram " << endl;
	string szyfrogram;
	cin >> setw(8) >> szyfrogram;
	szyfrogram = zmien_na_bity(szyfrogram, 8);
	szyfrogram = Permutacja(szyfrogram, index.PWw, 8);
	szyfrogram = runda(szyfrogram, klucz2);
	cout << "Wynik deszyfrowania kluczem drugiej rundy: " << szyfrogram << endl;
	szyfrogram = permutacja(szyfrogram, index.cross, 8);
	szyfrogram = runda(szyfrogram, klucz1);
	cout << "Wynik deszyfrowania kluczem pierwszej rundy: " << szyfrogram << endl;
	szyfrogram = permutacja(szyfrogram, index.PO, 8);
	string odszyfrogram = szyfrogram;
	cout << "Odszyfrowana wiadomosc: " << odszyfrogram << endl;

	keep_window_open("1");
	return 0;
}


string Permutacja(string wyraz, int *index, int n)
{
	char *tab = new char[n];
	for (int i = 0; i < n; i++)
	{
		tab[i] = wyraz[index[i]];
	}
	string wynik = "";


	for (int i = 0; i < n; i++)
	{
		wynik += tab[i];
	}
	return wynik;
}
string runda(string wyraz, string klucz)
{
	SBoxy sbox;
	indexy index;
	string w0 = "", w1 = "";
	for (int i = 0; i < 8; i++)
	{
		if (i > 3) w1 += wyraz[i];
		else w0 += wyraz[i];
	}
	string kopia_w1 = w1;
	w1 = permutacja(w1, index.P4w8, 8);
	w1 = Xor(w1, klucz, 8);
	string w1_half1 = "", w1_half2 = "";
	for (int i = 0; i < 8; i++)
	{
		if (i > 3) w1_half2 += w1[i];
		else w1_half1 += w1[i];
	}
	w1 = sboks(w1_half1, sbox.SBox1, sbox.check) + sboks(w1_half2, sbox.SBox2, sbox.check);
	w1 = permutacja(w1, index.P4, 4);
	w1 = Xor(w0, w1, 4);
	wyraz = w1 + kopia_w1;
	return wyraz;
}
string zmien_na_bity(string wyraz, int n)
{
	int x;
	for (int i = 0; i < wyraz.length(); i++)
	{
		if (wyraz[i] != '0' && wyraz[i] != '1')
		{
			x = rand() % 2 + 1;
			if (x == 1) wyraz[i] = '1';
			else wyraz[i] = '0';
		}
	}
	if (wyraz.length() < n)
	{
		for (int i = wyraz.length(); i < n; i++)
		{
			wyraz += '0';
		}
	}
	return wyraz;
}
string Xor(string wyraz, string klucz, int n)
{
	string wynik = "";
	for (int i = 0; i <n; i++)
	{
		if (wyraz[i] == klucz[i]) wynik += '0';
		else wynik += '1';
	}
	return wynik;
}
string sboks(string half, int box[][4], int check[][2])
{
	int cx = half[1] - '0', cy = (int)half[2] - '0', rx = (int)half[0] - '0', ry = (int)half[3] - '0';
	int c = check[cx][cy];
	int r = check[rx][ry];
	int revx = box[r][c];
	int x, y;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (check[i][j] == revx) { x = i; y = j; }
		}
	}
	stringstream ss;
	ss << x << y;
	string wynik = ss.str();
	return wynik;
}
