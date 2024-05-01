#include<bits/stdc++.h>
using namespace std;
#define PATH "/Users/sudhanshu/dm_project/COL-CUBE/FileStorage/"
map<int,string> mp;
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
        queriesMenu();
    }
    string FileName=PATH;
    while(!s.empty()){
        int a=*s.begin();
        FileName=FileName+mp[a]+"-";
        s.extract(a);
    }
    FileName.pop_back();
    FileName+=".text";
    cout<<FileName<<"\n";
    string queryFile="query_output.txt";
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
    queriesMenu();
}