#include <bits/stdc++.h>
#include <sys/stat.h>

using namespace std;

struct Attribute
{
    int length;
    char type[20];
    char values[20][50];
};

// string root = "/Users/sudhanshu/dm_project/COL-CUBE/cubestore/";
// string rootFolder = "/home/arch/D/dm/";
string rootFolder = "/Users/sudhanshu/dm_project/COL-CUBE/";
string fileStorageFolder = rootFolder + "FileStorage/";
string attrFilePath = rootFolder + "dim_attr.bin";
string columnFolder = rootFolder + "cubestore/fact_sales/";
int cnt = 0;
vector<vector<string>> combinations;
vector<string> attributes;
vector<string> facts;
vector<string> aggregates;
map<string, map<string, map<string, double>>> group;
map<string, vector<double>> factsData;
std::chrono::duration<double> time = std::chrono::duration<double>::zero();
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
    vector<string> fileNames;
    string combination = fileStorageFolder;
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
    combination += ".csv";
    cerr<<combination<<"\n";
    freopen(combination.c_str(), "w", stdout);
    vector<string> combString;
    string fileName = columnFolder + fileNames[0];
    ifstream inputFile(fileName, ios::binary | ios::in);
    if (!inputFile.is_open())
    {
        cerr << "Error opening file: " << fileName << endl; // Skip to the next file if unable to open
    }

    string line;
    while (getline(inputFile, line, '\0'))
    {
        // cout << line << endl;  // Write each line to stdout
        combString.push_back(line);
    }

    inputFile.close(); // Close the input file
    for (int i = 1; i < numberOfCombinations; i++)
    {
        string fileName = columnFolder + fileNames[i];
        ifstream inputFile(fileName, ios::binary | ios::in);
        if (!inputFile.is_open())
        {
            cerr << "Error opening file: " << fileName << endl;
            continue; // Skip to the next file if unable to open
        }

        string line;
        int j = 0;
        while (getline(inputFile, line, '\0'))
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
    std::chrono::duration<double> startHere = std::chrono::high_resolution_clock::now();
    for (const auto& outer_pair : group)
    {
        for (const auto& inner_pair : outer_pair.second)
        {
            for (const auto& inner_inner_pair : inner_pair.second)
            {
                cout << outer_pair.first << "," << inner_pair.first << "," << inner_inner_pair.first
                    << "," << inner_inner_pair.second << endl;
            }
        }
    }
    std::chrono::duration<double> endHere = std::chrono::high_resolution_clock::now();
    time+=endHere-startHere;
    group.clear();
    cerr << cnt << "\n";
    fclose(stdout);
}

int main()
{
    // ios::sync_with_stdio(false);
    // cin.tie(nullptr);
    ifstream attrFile(attrFilePath, ios::binary | ios::in);
    // freopen("output.txt", "wb", stdout);
    clock_t tStart = clock();
    auto start = std::chrono::high_resolution_clock::now();
    Attribute attribute;

    // Read data loop with error handling
    while (attrFile.read(reinterpret_cast<char*>(&attribute), sizeof(attribute))) {
        if (attrFile.fail()) {
            cerr << "Error reading attribute length." << endl;
            break;
        }
        string type(attribute.type);
        for (int i = 0; i < attribute.length; i++) {
            if (type == "dim") {
                attributes.push_back(attribute.values[i]);
            } else if (type == "fact") {
                facts.push_back(attribute.values[i]);
            } else if (type == "aggr") {
                aggregates.push_back(attribute.values[i]);
            }
        }
    }
    attrFile.close();
    // cerr<<aggregates.size()<<"\n";
    for (int i = 0; i < facts.size(); i++)
    {
        vector<double> values;
        string fileName = columnFolder + facts[i] + ".dat";
        ifstream inputFile(fileName, ios::binary | ios::in);
        if (!inputFile.is_open())
        {
            cerr << "Error opening file: " << fileName << endl;
            continue; // Skip to the next file if unable to open
        }
        string line;
        int limit = 0;
        while (getline(inputFile, line, '\0'))
        {
            // cout << line << endl;  // Write each line to stdout
            if (line == "NA") line = "0";
            // cerr<<line<<"\n";
            double num = stod(line);
            values.push_back(num);
            // limit++;
        }

        inputFile.close();
        factsData[facts[i]] = values;
    }
    
    auto makeCombinations = [&]()
        {
            int total = pow(2, attributes.size());
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
    // cout << combinations.size() << "\n";
    // for (int i = 0; i < combinations.size(); i++)
    // {
    //     cout << combinations[i].size() << " ";
    //     for (int j = 0; j < combinations[i].size(); j++)
    //     {
    //         cout << combinations[i][j] << " ";
    //     }
    //     cout << "\n";
    // }
    // freopen("output3.txt", "wb", stdout);
    mkdir(fileStorageFolder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    // for (int index = 0; index < 3; index++)
    for (int index = 0; index < combinations.size(); index++)
    {
        doOperations(index);
    }
    // std::this_thread::sleep_for(std::chrono::seconds(2));

    // Stop measuring time
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    std::chrono::duration<double> duration = end - start- time;
    cerr << "Time taken by the program: " << duration.count() << " seconds" << std::endl;
}