#include <fstream>
#include <string>
#include <sstream>

using namespace std;

int main()
{
    fstream input("FirstNames.txt");
    ofstream outputEng("LastNamesM.txt");
    ofstream outputRus("LastNamesF.txt");
    string line;
    while (getline(input, line)) 
    {
        stringstream line_stream(line);
        string temp;
        line_stream >> temp;
        outputEng << temp << '\n';
        line_stream >> temp;
        outputRus << temp << '\n';
    }

    input.close();
    outputEng.close();
    outputRus.close();

    return 0;
}