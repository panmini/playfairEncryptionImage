#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>


int ChineseRemainderTheorem(int a, int b, int m1, int m2);
void Decryption(int Cipher);
int inverse(int a, int b);
int Encryption(int PlainText);
int KeyGeneration();
int FastExponention(int bit, int n, int* y, int* a);
int FindT(int a, int m, int n);
int PrimarityTest(int a, int i);
int gcd(int a, int b);

int e = 2, n;
int p, q;


int main(int argc, char** argv)
{
	if(*argv[1] == 'e')
	{
		//key i encrypte edecez
		//p,q ve n setlemeyi unutma	


		n = atoi(argv[2]);

		for (size_t i = 3; i < 259; i++)
		{
			printf("%d ",Encryption( atoi(argv[i]) ) );
		}
		
	}
	else if (*argv[1] == 'd')
	{
		//key i decrypte edecez
		//p,q ve n setlemeyi unutma

		p = atoi(argv[2]); q = atoi(argv[3]); n = p*q;
	
		for (size_t i = 4; i < 260; i++)
		{
			Decryption(atoi(argv[i]));
		}
	}
	else if (*argv[1] == 'k')
	{
		KeyGeneration();	
	}
	

	
	/*int PlainText;
	KeyGeneration();

	do {
		PlainText = rand() % (n - 1) + 1; // PlainText belongs to the group Zn
	}
	while (gcd(PlainText, n) != 1); // PlainText belongs to the group Z*n

	printf("Plain text -- %d\n", PlainText);
	int Cipher = Encryption(PlainText);
	Decryption(Cipher);
 */
	
	
	return 0;
}

int gcd(int a, int b)
{
	int q, r1, r2, r;

	if (a > b) {
		r1 = a;
		r2 = b;
	}
	else {
		r1 = b;
		r2 = a;
	}

	while (r2 > 0) {
		q = r1 / r2;
		r = r1 - q * r2;
		r1 = r2;
		r2 = r;
	}

	return r1;
}

int PrimarityTest(int a, int i)
{
	int n = i - 1;
	int k = 0;
	int j, m, T;

	while (n % 2 == 0) {
		k++;
		n = n / 2;
	}

	m = n;
	T = FindT(a, m, i);

	if (T == 1 || T == i - 1)
		return 1;

	for (j = 0; j < k; j++)
	{
		T = FindT(T, 2, i);

		if (T == 1)
			return 0;

		if (T == i - 1)
			return 1;
	}

	return 0;
}

int FindT(int a, int m, int n)
{
	int r;
	int y = 1;
	
	while (m > 0) {
		r = m % 2;
		FastExponention(r, n, &y, &a);
		m = m / 2;
	}
	
	return y;
}

int FastExponention(int bit, int n, int* y, int* a)
{
	if (bit == 1)
		*y = (*y * (*a)) % n;

	*a = (*a) * (*a) % n;
}
int KeyGeneration()
{
	do {
		do {
			p = rand() % 500;
		}
		while (!((p % 2 == 1) && (p % 4 == 3)));
	}
	while (!PrimarityTest(2, p));

	do {
		do {
			q = rand() % 500;
		}
		while (!((q % 2 == 1) && (q % 4 == 3)));
	}
	while (!PrimarityTest(2, q));
	
	n = p * q;
	printf("%d %d %d", p, q, n);
}
int Encryption(int PlainText)
{
	//printf("PlainText is %d\n", PlainText);
	int cipher = FindT(PlainText, e, n);
	//fprintf(stdout, "Cipher Text is %d\n", cipher);
	
	return cipher;
}
int inverse(int a, int b)
{
	int inv;
	int q, r, r1 = a, r2 = b, t, t1 = 0, t2 = 1;

	while (r2 > 0) {
		q = r1 / r2;
		r = r1 - q * r2;
		r1 = r2;
		r2 = r;

		t = t1 - q * t2;
		t1 = t2;
		t2 = t;
	}

	if (r1 == 1)
		inv = t1;
	if (inv < 0)
		inv = inv + a;

	return inv;
}

void Decryption(int Cipher)
{
	int P1, P2, P3, P4;
	int a1, a2, b1, b2;

	a1 = FindT(Cipher, (p + 1) / 4, p);
	a2 = p - a1;

	b1 = FindT(Cipher, (q + 1) / 4, q);
	b2 = q - b1;

	P1 = ChineseRemainderTheorem(a1, b1, p, q);
	P2 = ChineseRemainderTheorem(a1, b2, p, q);
	P3 = ChineseRemainderTheorem(a2, b1, p, q);
	P4 = ChineseRemainderTheorem(a2, b2, p, q);

	printf("%d %d %d %d ", P1, P2, P3, P4);
}

int ChineseRemainderTheorem(int a, int b, int m1, int m2)
{
	int M, M1, M2, M1_inv, M2_inv;
	int result;
 
	M = m1 * m2;
	M1 = M / m1;
	M2 = M / m2;
 
	M1_inv = inverse(m1, M1);
	M2_inv = inverse(m2, M2);
 
	result = (a * M1 * M1_inv + b * M2 * M2_inv) % M;
 
	return result;

}
