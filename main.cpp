#include <bits/stdc++.h>

using namespace std;
#define V 50

class node             //contains the details about the bus
{
public:
    int busno;
    int cost;
    int sTime;
    int eTime;
    //node(){}
    node(int bno, int s1, int e1, int c)
    {
        busno = bno;
        sTime = s1, eTime = e1;
        cost = c;
    }
};

class Travel          //it contains all the member variables and functions containing information about the routes
{
private:
    int source, destination;
    int minCost, minTime;
    vector<int> minCostBus;
    vector<int> BustartTime;
    vector<int> BusendTime;
    vector<vector<int>> allpaths;
    vector<int> minTimeBus;
    vector<int> minCostbestPath;
    vector<int> minTimebestPath;
    map<int, string> cities;
    vector<int> adj[V];
    int visited[1000] = {0};
    vector<node> bus[V][V];

public:
    void mapCities();
    void addEdge(int u, int v);
    void sendData();
    void findAllPaths(int u, int v, vector<int> path);
    void addbus(int bno, int u, int v, int s, int e, int c);
    void initializeStartData(string s, string d);
    void printMinCost();
    void printMinTime();
    void UserChoice(int startTime);
    void MinCostUtil(int startTime, int sIndex, int dIndex, int pathIndex, int cost, int n, vector<int> busId,vector<int> busst,vector<int> buset);
    void MinTimeUtil(int startTime, int sIndex, int dIndex, int pathIndex, int cost, int n, vector<int> busId,vector<int> busst,vector<int> buset);
};

void Travel::addbus(int bno, int u, int v, int s, int e, int c) // function for adding a new bus
{

    bus[u][v].push_back(node(bno, s, e, c));
    bus[v][u].push_back(node(bno, s, e, c));
}

void Travel::mapCities()   //map cities to a particular vertex
{
    cities[0] = "DELHI";
    cities[1] = "NOIDA";
    cities[2] = "GHAZIABAD";
    cities[3] = "GURUGRAM";
    cities[4] = "SONEPAT";
    cities[5] = "KANPUR";
    cities[6] = "MATHURA";
    cities[7] = "MORADABAD";
    cities[8] = "LUCKNOW";
    cities[9] = "AGRA";
    cities[10] = "JAIPUR";
    cities[11] = "MEERUT";
    cities[12] = "CHANDIGARH";
    cities[13] = "AMBALA";
    cities[14] = "DEHRADUN";
    cities[15] = "PRAYAGRAJ";
}

void Travel::sendData()     //function for taking input from the user
{
    cout<<"---------------Welcome---------------"<<endl;
    cout << "Enter the city from which u want to take the Bus:" << endl;
    string u, v;
    cin >> u;
    cout << "Enter the destination city:" << endl;
    cin >> v;
    initializeStartData(u, v);
}

void Travel::initializeStartData(string s, string d)
{
    transform(s.begin(), s.end(), s.begin(), ::toupper); //converts to UpperCase
    transform(d.begin(), d.end(), d.begin(), ::toupper);
    source=destination=-1;
    for (auto it : cities)  //finds the vertex number for the cities
    {
        if (it.second == s)
        {
            source = it.first;
            break;
        }
    }
    for (auto it : cities)
    {
        if (it.second == d)
        {
            destination = it.first;
            break;
        }
    }
    if(source==-1 || destination==-1)
    {
        cout<<"Sorry we have no buses running between these cities:"<<endl;
        return;
    }
    cout<<"Enter the time when you want to leave for your destination in 24 hour format:"<<endl;
    int startTime;
    cin>>startTime;
    if(startTime>24 || startTime<1)
    {
        cout<<"Please enter the time in 24 hour format!!"<<endl<<endl;
        return;
    }
    vector<int> path;
    path.push_back(source);
    allpaths.clear();
    findAllPaths(source, destination, path);
    if(allpaths.size()==0)
    {
        cout<<"No path possible to destination:"<<endl;
        return;
    }
    /*for (auto it : allpaths)
    {

        for (auto it2 : it)
            cout << "->" << cities[it2];
        cout << endl;
    }*/
    UserChoice(startTime);
}

void Travel::addEdge(int u, int v)  // function for adding a new edge
{
    adj[u].push_back(v);
    adj[v].push_back(u);
}

void Travel::UserChoice(int startTime)  //asks the users about their choice
{
    minCost = INT_MAX;
    minTime = INT_MAX;
    vector<int> busId;
    vector<int> busst;
    vector<int> buset;
    cout << "You have two options to get to your destinations"<<endl<<endl;
    cout << "1) Do u want take the path that takes you to the destination in minimum cost " << endl;
    cout << "2) Do u want take the path that takes you to the destination in minimum time " << endl;
    cout << "Which option would u like?" << endl<<endl;
    int ch;
    cin >> ch;
    if (ch == 1)
    {
        for (int i = 0; i < allpaths.size(); i++) // traverse the path list
            MinCostUtil(startTime, 0, 1, i, 0, allpaths[i].size(), busId,busst,buset);
        printMinCost();
    }
    else
    {
        for (int i = 0; i < allpaths.size(); i++) // traverse the path list
            MinTimeUtil(startTime, 0, 1, i, 0, allpaths[i].size(), busId,busst,buset);
        printMinTime();
    }
    int n;
    cout<<endl<<"Do you want to again look for a bus route:1-continue,2-exit"<<endl;
    cin>>n;
    if(n==1)
        sendData();
    else
        return;

}

void Travel::MinCostUtil(int startTime, int sIndex, int dIndex, int pathIndex, int cost, int n,vector<int> busId,vector<int> busst,vector<int> buset)
{
    if (dIndex == n)                          //function for finding path with the minimum cost
    {
        if (minCost>=cost)
        {
            if(minCost==cost)
            {
                if(minTime>startTime)
                {
                    minTime = startTime;
                    minCostBus=busId;
                    minCostbestPath=(allpaths[pathIndex]);
                    BustartTime=busst;
                    BusendTime=buset;
                }
            }
            else{
            minCost = cost;
            minTime = startTime;
            minCostBus=busId;
            minCostbestPath=(allpaths[pathIndex]);
            BustartTime=busst;
            BusendTime=buset;
            }
        }
        return;
    }
    else
    {
        for (auto Busavail : bus[allpaths[pathIndex][sIndex]][allpaths[pathIndex][dIndex]])
        {
            if (Busavail.sTime >= startTime)
            {
                busId.push_back(Busavail.busno);
                busst.push_back(Busavail.sTime);
                buset.push_back(Busavail.eTime);
                MinCostUtil(Busavail.eTime, sIndex + 1, dIndex + 1, pathIndex, cost + Busavail.cost, n, busId,busst,buset);
                busId.pop_back();
                busst.pop_back();
                buset.pop_back();
            }
        }
    }
}

void Travel::MinTimeUtil(int startTime, int sIndex, int dIndex, int pathIndex, int cost, int n,vector<int> busId,vector<int> busst,vector<int> buset)
{
    if (dIndex == n)                 //function for finding path with the minimum time
    {
        if (minTime >= startTime)
        {
            if(minTime==startTime)
            {
                if(minCost>cost)
                {
                    minCost=cost;
                    minTimeBus=busId;
                    minTimebestPath=(allpaths[pathIndex]);
                    BustartTime=busst;
                    BusendTime=buset;
                }
            }
            else{
            minTime = startTime;
            minCost = cost;
            minTimeBus=busId;
            minTimebestPath=(allpaths[pathIndex]);
            BustartTime=busst;
            BusendTime=buset;
            }
        }
        return;
    }
    else
    {
        for (auto Busavail : bus[allpaths[pathIndex][sIndex]][allpaths[pathIndex][dIndex]])
        {
            if (Busavail.sTime >= startTime)
            {
                busId.push_back(Busavail.busno);
                busst.push_back(Busavail.sTime);
                buset.push_back(Busavail.eTime);
                MinTimeUtil(Busavail.eTime, sIndex + 1, dIndex + 1, pathIndex, cost + Busavail.cost, n, busId,busst,buset);
                busId.pop_back();
                busst.pop_back();
                buset.pop_back();
            }
        }
    }
}

void Travel::printMinTime()
{
    if (minTimeBus.size()==0)
    {
        cout << "No combination of buses exist that can take to you to your destination" << endl;
        return;
    }
    cout << "Following is the best path available that will take you to your destination in Minimum time:" << endl;
    string s="",s2="";
    for (auto it : minTimebestPath)
    {
        s=cities[it];
        cout << "->" <<s ;
    }
    cout << endl; int i = 0;
    int j = 1;
     cout << "Following buses you should take to reach to your destination" << endl<<endl;
     int k=0;
    for (auto it : minTimeBus)
    {
        s=cities[minTimebestPath[i]];
        s2=cities[minTimebestPath[j]];
        cout<<endl;
        cout<<"Take BusNumber-" << it << " from " << s << " which is scheduled at "<<BustartTime[k]<<"hrs and reach "<< s2<<" at "<<BusendTime[k++]<<"hrs"<<endl;
        i++;
        j++;
        cout<<endl;
    }
    cout<<endl<<endl;
    int l=minTimebestPath.size();
    cout << "You will reach " <<cities[minTimebestPath[l-1]]<<" at "<<minTime<<"hrs"<<endl;
    cout << "Your total Bus fare = " << minCost<<endl;
}

void Travel::printMinCost()
{
    if (minCostBus.size()==0)
    {
        cout << "No combination of buses exist that can take to you to your destination" << endl;
        return;
    }
    cout << "Following is the best path available that will take you to your destination in Minimum Cost:" << endl;
    string s="",s2="";
    for (auto it : minCostbestPath)
    {
        s=cities[it];
        cout << "->" << (s);
    }
    cout << endl;
    cout << "Following buses u should take to reach to your destination" << endl<<endl;
    int i = 0;
    int j = 1;
    int k=0;
    for (auto it : minCostBus)
    {
        s=cities[minCostbestPath[i]];
        s2=cities[minCostbestPath[j]];
        cout<<endl;
         cout <<"Take BusNumber-" << it << " from " << s << " which is scheduled at "<<BustartTime[k]<<"hrs and reach "<< s2<<" at "<<BusendTime[k++]<<"hrs";
        i++;
        j++;
        cout<<endl;
    }
    cout << endl<<endl;
    int l=minCostbestPath.size();
    cout << "You will reach " <<cities[minCostbestPath[l-1]]<<" at "<<minTime<<"hrs"<<endl;
    cout << "Your total Bus fare = " << minCost<<endl;
}

void Travel::findAllPaths(int u, int v, vector<int> path)  //function for finding all possible paths
{
    visited[u] = 1;
    if (u == v)
    {
        allpaths.push_back(path);
    }
    else
    {
        for (int i = 0; i < adj[u].size(); i++)
        {
            int y = adj[u][i];
            if (visited[y] == 0)
            {
                visited[y] = 1;

                path.push_back(y);
                findAllPaths(y, v, path);
                path.pop_back();
            }
        }
    }
    visited[u] = 0;
}

int main()
{
    Travel obj;
    obj.mapCities();
    obj.mapCities();
    obj.addEdge(0, 1);
    obj.addEdge(0, 2);
    obj.addEdge(0, 3);
    obj.addEdge(0, 4);
    obj.addEdge(1, 2);
    obj.addEdge(1, 6);
    obj.addEdge(1, 5);
    obj.addEdge(2, 7);
    obj.addEdge(3, 4);
    obj.addEdge(3, 10);
    obj.addEdge(5, 7);
    obj.addEdge(5, 8);
    obj.addEdge(5, 9);
    obj.addEdge(6, 9);
    obj.addEdge(7, 8);
    obj.addEdge(9, 10);
    obj.addEdge(2, 11);
    obj.addEdge(11, 12);
    obj.addEdge(4, 13);
    obj.addEdge(12, 13);
    obj.addEdge(11, 13);
    obj.addEdge(11, 14);
    obj.addEdge(12, 14);
    obj.addEdge(5, 15);
    obj.addEdge(8, 15);
     obj.addbus(101, 0, 1, 6, 7, 50);
    obj.addbus(102, 0, 1, 12, 13, 60);
    obj.addbus(103, 0, 1, 17, 18, 80);
    obj.addbus(104, 0, 2, 9, 10, 40);
    obj.addbus(105, 0, 2, 14, 15, 60);
    obj.addbus(106, 0, 3, 5, 6, 30);
    obj.addbus(107, 0, 3, 15, 16, 50);
    obj.addbus(108, 0, 4, 10, 12, 40);
    obj.addbus(109, 1, 2, 8, 9, 20);
    obj.addbus(110, 1, 2, 12, 13, 30);
    obj.addbus(111, 1, 2, 16, 17, 40);
    obj.addbus(112, 1, 5, 7, 10, 80);
    obj.addbus(113, 1, 5, 9, 12, 100);
    obj.addbus(114, 1, 6, 10, 12, 60);
    obj.addbus(115, 1, 6, 18, 20, 20);
    obj.addbus(116, 2, 7, 7, 10, 70);
    obj.addbus(117, 2, 7, 14, 17, 90);
    obj.addbus(118, 2, 7, 11, 14, 60);
    obj.addbus(119, 7, 8, 12, 16, 150);
    obj.addbus(120, 5, 8, 8, 10, 80);
    obj.addbus(121, 5, 8, 15, 17, 70);
    obj.addbus(122, 5, 8, 17, 19, 90);
    obj.addbus(123, 6, 9, 12, 13, 50);
    obj.addbus(124, 6, 9, 15, 16, 40);
    obj.addbus(125, 5, 9, 6, 9, 20);
    obj.addbus(126, 5, 9, 13, 14, 40);
    obj.addbus(127, 5, 9, 17, 20, 80);
    obj.addbus(128, 3, 10, 10, 13, 70);
    obj.addbus(129, 3, 10, 15, 18, 60);
    obj.addbus(130, 3, 4, 12, 14, 40);
    obj.addbus(131, 9, 10, 10, 13, 90);
    obj.addbus(132, 3, 10, 5, 8, 70);
    obj.addbus(133, 3, 0, 9, 10, 40);
    obj.addbus(134, 0, 2, 11, 12, 50);
    obj.addbus(135, 4, 0, 7, 8, 30);
    obj.addbus(136, 7, 8, 16, 20, 50);
    obj.addbus(137, 1, 5, 13, 17, 60);
    obj.addbus(138, 8, 5, 6, 9, 50);
    obj.addbus(139, 1, 6, 13, 15, 40);
    obj.addbus(140, 9, 10, 12, 15, 60);
    obj.addbus(141, 2, 11, 9, 11, 40);
    obj.addbus(142, 2, 11, 12, 14, 50);
    obj.addbus(143, 2, 11, 16, 18, 60);
    obj.addbus(144, 11, 12, 6, 10, 50);
    obj.addbus(145, 11, 12, 14, 16, 70);
    obj.addbus(146, 4, 13, 8, 11, 50);
    obj.addbus(147, 4, 13, 11, 14, 50);
    obj.addbus(148, 4, 13, 13, 16, 60);
    obj.addbus(149, 12, 13, 6, 8, 50);
    obj.addbus(150, 12, 13, 12, 14, 30);
    obj.addbus(151, 11, 13, 13, 16, 50);
    obj.addbus(152, 11, 14, 9, 12, 30);
    obj.addbus(153, 11, 14, 11, 14, 50);
    obj.addbus(154, 11, 14, 14, 17, 40);
    obj.addbus(155, 12, 14, 6, 10, 50);
    obj.addbus(156, 12, 14, 11, 15, 60);
    obj.addbus(157, 8, 7, 6, 10, 40);
    obj.addbus(158, 11, 12, 18, 21, 60);
    obj.addbus(159, 2, 11, 13, 14, 40);
    obj.addbus(160, 8, 15, 7, 10, 40);
    obj.addbus(160, 8, 15, 14, 17, 60);
    obj.addbus(160, 8, 15, 19, 22, 70);
    obj.addbus(168, 1, 5, 10, 13, 60);
    obj.addbus(161, 5, 15, 6, 9, 30);
    obj.addbus(161, 5, 15, 13, 16, 50);
    obj.addbus(161, 5, 15, 17, 20, 40);
    obj.addbus(162, 11, 2, 10, 11, 50);
    obj.addbus(163, 3, 0, 8, 9, 40);
    obj.addbus(164, 0, 1, 6, 8, 10);
    obj.addbus(165, 10, 3, 6, 8, 80);
    obj.addbus(166, 3, 0, 8, 9, 50);
    obj.addbus(167, 0, 1, 9, 10, 30);
    obj.sendData();
    cout << endl;
}


