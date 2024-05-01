#include <bits/stdc++.h>
#include <sys/stat.h>
using namespace std;
vector<vector<string>> combinations;
vector<string> attributes;
vector<string> facts;
vector<string> aggregates;
map<string, map<string, map<string, double>>> group;
string path = "/Users/sudhanshu/dm_project/COL-CUBE/FileStorage/";
int cnt=0;
map<string,vector<double>> factsData;
void doCalculations(string comb, string fact, string aggr, double val)
{
    if (aggr == "sum")
    {
        group[comb][fact][aggr] += val;
    }
    else if (aggr == "max")
    {
        if (group.find(comb) == group.end() || group[comb].find(fact) == group[comb].end() || group[comb][fact].find(aggr) == group[comb][fact].end())
            group[comb][fact][aggr] = INT_MIN;
        group[comb][fact][aggr] = max(val, group[comb][fact][aggr]);
    }
    else if (aggr == "min")
    {
        if (group.find(comb) == group.end() || group[comb].find(fact) == group[comb].end() || group[comb][fact].find(aggr) == group[comb][fact].end())
            group[comb][fact][aggr] = INT_MAX;
        group[comb][fact][aggr] = min(val, group[comb][fact][aggr]);
    }
}
void doOperations(int index)
{
    cnt++;
    int numberOfCombinations = combinations[index].size();
    int numFiles = numberOfCombinations + facts.size();
    ifstream *filePointers[numFiles];
    vector<string> fileNames;
    string combination = path;
    for (int i = 0; i < numberOfCombinations; i++)
    {
        string fileName = combinations[index][i];
        combination += fileName;
        if (i != numberOfCombinations - 1)
        {
            combination += "-";
        }
        fileName += ".dat";
        fileNames.push_back(fileName);
    }
    combination += ".text";
    freopen(combination.c_str(), "wb", stdout);
    vector<string> combString;
    string fileName = "/Users/sudhanshu/dm_project/COL-CUBE/cubestore/fact_sales/" + fileNames[0];
    ifstream inputFile(fileName);
    if (!inputFile.is_open())
    {
        cerr << "Error opening file: " << fileName << endl; // Skip to the next file if unable to open
    }

    string line;
    while (getline(inputFile, line))
    {
        // cout << line << endl;  // Write each line to stdout
        combString.push_back(line);
    }

    inputFile.close(); // Close the input file
    for (int i = 1; i < numberOfCombinations; i++)
    {
        string fileName = "/Users/sudhanshu/dm_project/COL-CUBE/cubestore/fact_sales/" + fileNames[i];
        ifstream inputFile(fileName);
        if (!inputFile.is_open())
        {
            cerr << "Error opening file: " << fileName << endl;
            continue; // Skip to the next file if unable to open
        }

        string line;
        int j = 0;
        while (getline(inputFile, line))
        {
            // cout << line << endl;  // Write each line to stdout
            combString[j] = combString[j] + " " + line;
            j++;
        }

        inputFile.close(); // Close the input file
    }
    // freopen("test.txt","w",stdout);
    for (int k = 0; k < combString.size(); k++)
    {
        // cout << combString[i] << "\n";
        for (int i = 0; i < facts.size(); i++)
        {
            for (int j = 0; j < aggregates.size(); j++)
            {
                doCalculations(combString[k], facts[i], aggregates[j], factsData[facts[i]][k]);
            }
        }
    }
    for (const auto &outer_pair : group)
    {
        for (const auto &inner_pair : outer_pair.second)
        {
            for (const auto &inner_inner_pair : inner_pair.second)
            {
                cout << outer_pair.first << " " << inner_pair.first << " " << inner_inner_pair.first
                     << " " << inner_inner_pair.second << endl;
            }
        }
    }
    group.clear();
    cerr<<cnt<<"\n";
    fclose(stdout);
}
int main()
{
    	ios::sync_with_stdio(false);
  cin.tie(nullptr);
    freopen("dim_attr.txt", "r", stdin);
    freopen("output.txt", "wb", stdout);
    clock_t tStart = clock();
    auto start = std::chrono::high_resolution_clock::now();
    int numberOfAttributes, numberOfFacts, numberOfAggregates;
    cin >> numberOfAttributes;
    attributes.resize(numberOfAttributes);
    for (string &s : attributes)
    {
        cin >> s;
    }
    cin >> numberOfFacts;
    facts.resize(numberOfFacts);
    for (string &s : facts)
    {
        cin >> s;
    }
    for (int i = 0; i < facts.size(); i++)
        {
            vector<double> values;
            string fileName = "/Users/sudhanshu/dm_project/COL-CUBE/cubestore/fact_sales/" + facts[i] + ".dat";
            ifstream inputFile(fileName);
            if (!inputFile.is_open())
            {
                cerr << "Error opening file: " << fileName << endl;
                continue; // Skip to the next file if unable to open
            }

            string line;
            while (getline(inputFile, line))
            {
                // cout << line << endl;  // Write each line to stdout
                if(line=="NA") line="0";
                double num = stod(line);
                values.push_back(num);
            }

            inputFile.close(); 
            factsData[facts[i]]=values;
        }
    cin >> numberOfAggregates;
    aggregates.resize(numberOfAggregates);
    for (string &s : aggregates)
    {
        cin >> s;
    }
    auto makeCombinations = [&]()
    {
        int total = pow(2, numberOfAttributes);
        for (int i = 1; i < total; i++)
        {
            vector<string> tempComb;
            for (int j = 0; j < 31; j++)
            {
                if ((i >> j) & 1)
                {
                    tempComb.push_back(attributes[j]);
                }
            }
            combinations.push_back(tempComb);
        }
        return;
    };
    makeCombinations();
    cout << combinations.size() << "\n";
    for (int i = 0; i < combinations.size(); i++)
    {
        cout << combinations[i].size() << " ";
        for (int j = 0; j < combinations[i].size(); j++)
        {
            cout << combinations[i][j] << " ";
        }
        cout << "\n";
    }
    freopen("output3.txt", "wb", stdout);
    mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    for (int index = 0; index < combinations.size(); index++)
    {
        doOperations(index);
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Stop measuring time
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    std::chrono::duration<double> duration = end - start;
    cerr << "Time taken by the program: " << duration.count() << " seconds" << std::endl;
}