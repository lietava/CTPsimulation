#include <iostream>
#include <string>
#include <list> 

using namespace std;

typedef short unsigned int INT;

typedef std::list<INT> evenList;
struct Box{
 list<INT> events;
};

int main()
{
 Box* qq = new Box[10];
 //eventList aa;
 //Box qq[10];
 qq[0].events.push_back(100);
 cout << qq[0].events.empty() << endl;;
}
