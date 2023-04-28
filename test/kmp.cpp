#include<iostream>
#include<string>
using namespace std;

// kmp 算法
void getNext(int* next, string& s)
{
	// initiliazed 
	int j = 0;
	next[0] = 0;
	for(int i = 1; i < s.size(); i++) {
		while(j > 0 && s[i] != s[j])
			j = next[j - 1];
		if(s[i] == s[j])
			j++;
		next[i] = j;
	}
}


int main()
{
	string t, p; 
	cout << "Input a text string: " << endl;
	cin >> t;	
	cout << "Input a pattern string: " << endl;
	cin >> p;
	int j = 0;
	int next[p.size()];
       	getNext(next, p);	
	for(int i = 0; i < t.size(); i++) {
		while(j > 0 && t[i] != p[j])
			j = next[j - 1];
		if(t[i] == p[j])
		       j++;
		if(j == p.size())
			cout << "The first match location is: " << (i - p.size() + 1) << endl;	
	}
	return 0;
}
