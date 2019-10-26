#include <iostream>
#include <string>
using namespace std;
string word, passage;
const auto lower = [](string& s) { for(auto &i:s)i=tolower(i); };
int main()
{
	cin >> word;
	cin.ignore(1);
	getline(cin, passage);
	lower(word);
	lower(passage);
	auto pos = passage.find(word);
	int times = 0;
	for (auto i = pos; i != string::npos; i = passage.find(word, i + word.size()))
		if (!(isalpha(passage[i + word.size()])||isalpha(passage[i-1])))
		{
			if (times == 0)
				pos = i;
			times++;
		}
	if (times)
		cout << times << " " << pos;
	else
		cout << -1;
	return 0;
}
