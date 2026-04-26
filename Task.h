#include <vector>
#include <string>
using namespace std;

struct Task{
    string description; 
    bool done;
};
void saveTasks(const vector<Task> tasks, const string& fileName);
vector<Task> loadTasksFromFile(const string& fileName);


