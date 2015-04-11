#ifdef __GNUC__
#include <ext/hash_map>
#else
#include <hash_map>
#endif

#include <iostream>
#include <string>


namespace std
{
using namespace __gnu_cxx;
}

using namespace std;

int main()
{
hash_map<string, int> hm;

string name1 = "abc";
string name2 = "abc";

hm[name1] = 1;
hm[name2] = 2;

cout<<hm[name1]<<endl;

return 0;
}