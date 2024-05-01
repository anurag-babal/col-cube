#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bits/stdc++.h>
#define MAX_LINE_LENGTH 512
#define MAX_TOKENS 23
#define MAX_COL_DATA 100
#define PATH "/Users/sudhanshu/dm_project/COL-CUBE/cubestore/fact_sales/"
#define MAX_PATH_LENGTH 256
using namespace std;
vector<string> files;
int numLines = 0;
void mapper(string colName)
{
    string FileNameWrite = PATH + colName + ".txt";
    string FileNameRead = PATH + colName + ".dat";
    ifstream file(FileNameRead);
    freopen(FileNameWrite.c_str(), "wb", stdout);
    map<string, vector<unsigned char>> stringToBit;
    // Check if the file is opened successfully
    if (!file.is_open())
    {
        std::cerr << "Error opening file." << std::endl;
        return ;
    }

    // Initialize a counter for the number of lines
    int lineCount = 0;

    // Read each line of the file
    std::string s;
    while (std::getline(file, s))
    {
        // Increment the line count for each line read
        if (stringToBit.find(s) == stringToBit.end())
            stringToBit[s] = vector<unsigned char>(10000, '0');
        stringToBit[s][lineCount] = '1';
        lineCount++;
    }

    // Close the file
    file.close();
    for (auto x : stringToBit)
    {
        cout << x.first << "\n";
        for (auto ch : x.second)
        {
            cout << ch;
        }
        cout << "\n";
    }
}
void makeBitMap()
{
    for (int i = 0; i < files.size(); i++)
    {
        cerr << files[i] << " ";
        mapper(files[i]);
    }
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    freopen("dim_attr.txt", "r", stdin);
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        string s;
        cin >> s;
        files.push_back(s);
    }
    makeBitMap();
    return 0;
}
