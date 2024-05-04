#include<bits/stdc++.h>
using namespace std;
#define PATH "/Users/sudhanshu/dm_project/COL-CUBE/exp/"
map<int,string> mp;
vector<string> split(const std::string& s, char delimiter) {
    vector<std::string> tokens;
    string token;
    istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
void parseQuery(string input,string &columnName,string &values){
    size_t pos = input.find('=');

    // Check if '=' is found
    if (pos != string::npos) {
        // Extract the substring before '='
        string key = input.substr(0, pos);
        // cout<<"key "<<key<<"\n";
        // Find the position of the first single quote after '='
        size_t startQuotePos = input.find('\'', pos);
        // cout<<startQuotePos<<"\n";

        // Find the position of the second single quote after '='
        size_t endQuotePos = input.find('\'', startQuotePos + 1);
        // cout<<endQuotePos<<"\n";


        // Check if both single quotes are found
        if (startQuotePos != string::npos && endQuotePos != string::npos) {
            // Extract the substring between the single quotes
            string value = input.substr(startQuotePos + 1, endQuotePos - startQuotePos - 1);

            columnName=key;
            values=value;
            // cout<<columnName<<" "<<value<<"\n";
        } else {
            cerr << "Invalid format: Missing single quote(s) around the value." << std::endl;
        }
    } else {
        // '=' not found
        cerr<< "No '=' character found in the string." << std::endl;
    }
}
void sliceMenu(){
    cout<<"Column name and value in format (Example: City='Bangalore')\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string query;
    getline(cin,query);
    cout<<query<<"\n";
    string FileName=PATH;
    FileName+="Customer_Name-City-State-Country-Customer_Segment-Order_ID-Order_Priority-Product_Category-Order_Date-Product_Sub_Category-Ship_Mode-Ship_Date.csv";
    string columnName,value;
    parseQuery(query,columnName,value);
    // cout<<columnName<<" "<<value<<"\n";
    freopen("query_output.csv","w",stdout);
    ifstream file(FileName.c_str());
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return ;
    }

    // Read the header line
    string header;
    getline(file, header);

    // Find the index of the "City" column
    istringstream headerStream(header);
    string column;
    size_t columnIndex = 0;
    while (getline(headerStream, column, ',')) {
        if (column == columnName) {
            break;
        }
        columnIndex++;
    }

    // Read each line from the file
    string line;
    while (getline(file, line)) {
        // Split the line into fields
        vector<string> fields = split(line, ',');
        // for(string s:fields){
        //     cout<<s<<" ";
        // }
        // cout<<"\n";
        // cout<<fields[columnIndex]<<" ";
        // cout<<(fields[columnIndex]==value?1:2)<<"\n";
        // cout<<(value.length())<<" "<<(fields[columnIndex].length())<<"\n";
        if (fields.size() > columnIndex && fields[columnIndex]==value) {
            cout << line <<endl;
        }
    }

    // Close the file
    file.close();
    fclose(stdout);

}
void queriesMenu(){
    cout<<"\nSelect the number of respective columns you want to group by \n";
    cout<<"1. Customer_Name\n2. City\n3. State\n4. Country\n5. Customer_Segment\n6. Order_ID\n7. Order_Priority\n8. Product_Category\n9. Order_Date\n10. Product_Sub_Category\n11. Ship_Mode\n12. Ship_Date\n-1. Finish\n";
    set<int> s;
    while(true){
        int op;
        cin>>op;
        if(op==-1 || op>12 || op<1){
            break;
        }
        s.insert(op);
    }
    if(s.size()==0){
        cout<<"Select only valid options\n";
        return;
    }
    string FileName=PATH;
    while(!s.empty()){
        int a=*s.begin();
        FileName=FileName+mp[a]+"-";
        s.extract(a);
    }
    FileName.pop_back();
    FileName+=".csv";
    cout<<FileName<<"\n";
    string queryFile="query_output.csv";
    freopen(queryFile.c_str(),"wb",stdout);
    ifstream inputFile(FileName.c_str()); // Open the input file
    if (!inputFile) {
        cerr << "Error: Unable to open input file." << std::endl;
        return ;
    }

    // ofstream outputFile(queryFile.c_str()); // Open the output file
    // if (!outputFile) {
    //     std::cerr << "Error: Unable to open output file." << std::endl;
    //     return ;
    // }
    string line;
    while (getline(inputFile, line)){
        cout<<line<<"\n";
    }

    // Close the files
    inputFile.close();
    // outputFile.close();
    fclose(stdout);
    cout<< "Query result is in query_output file" << std::endl;
}
void diceMenu(){
    cout<<"Column name and value in format (Example: City='Bangalore')\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string query1,query2;
    getline(cin,query1);
    cout<<query1<<"\n";
    //  cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout<<"Column name and value in format (Example: City='Bangalore')\n";
    getline(cin,query2);
    cout<<query2<<"\n";
    string FileName=PATH;
    FileName+="Customer_Name-City-State-Country-Customer_Segment-Order_ID-Order_Priority-Product_Category-Order_Date-Product_Sub_Category-Ship_Mode-Ship_Date.csv";
    string columnName1,value1,columnName2,value2;
    parseQuery(query1,columnName1,value1);
    parseQuery(query2,columnName2,value2);
    cout<<columnName1<<" "<<value1<<"\n";
    cout<<columnName2<<" "<<value2<<"\n";
    freopen("query_output.csv","w",stdout);
    ifstream file(FileName.c_str());
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return ;
    }

    // Read the header line
    string header;
    getline(file, header);

    // Find the index of the "City" column
    istringstream headerStream(header);
    string column;
    size_t columnIndex1 = 0,columnIndex2=0,colNo=0;
    while (getline(headerStream, column, ',')) {
        if (column == columnName1) {
            columnIndex1=colNo;
        }
        if(column==columnName2){
            columnIndex2=colNo;
        }
        colNo++;
    }

    // Read each line from the file
    string line;
    while (getline(file, line)) {
        // Split the line into fields
        vector<string> fields = split(line, ',');
        // for(string s:fields){
        //     cout<<s<<" ";
        // }
        // cout<<"\n";
        // cout<<fields[columnIndex]<<" ";
        // cout<<(fields[columnIndex]==value?1:2)<<"\n";
        // cout<<(value.length())<<" "<<(fields[columnIndex].length())<<"\n";
        if (fields.size() > columnIndex1 &&  fields.size() > columnIndex2 && fields[columnIndex1]==value1 && fields[columnIndex2]==value2) {
            cout << line <<endl;
        }
    }

    // Close the file
    file.close();
    fclose(stdout);

}
void menu(){
    cout<<"Choose query Type\n";
    cout<<"1. Slice\n2. Dice\n3. Rollup\n";
    int choice;
    cin>>choice;
    switch (choice)
    {
    case 1:
        sliceMenu();
        break;
    case 2:
        diceMenu();
        break;
    case 3:
        queriesMenu();
        break;
    default:
        break;
    }
}
int main(){
    mp[1]="Customer_Name";
    mp[2]="City";
    mp[3]="State";
    mp[4]="Country";
    mp[5]="Customer_Segment";
    mp[6]="Order_ID";
    mp[7]="Order_Priority";
    mp[8]="Product_Category";
    mp[9]="Order_Date";
    mp[10]="Product_Sub_Category";
    mp[11]="Ship_Mode";
    mp[12]="Ship_Date";
    menu();
}