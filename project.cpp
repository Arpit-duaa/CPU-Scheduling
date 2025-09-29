#include<iostream>
#include<iomanip>
#include<vector>
#include<queue>
using namespace std;

struct process{
    int id;
    int arrival_time=0;
    int burst_time;
    int priority;
};

template<typename T>
struct Node{
    T data;
    Node* next;

    Node(T val){
        data=val;
        next=nullptr;
    }
};

template<typename T>
struct Queue{
    Node<T>* tail;
    Node<T>* frontNode;

    Queue(){
        frontNode=tail=nullptr;
    }

    bool empty(){

        if(frontNode){
           return false;
        }
        else{
            return true;
        }

    }

    T front(){
        if(!empty()){
            return frontNode->data;
        }
        throw runtime_error("Queue is empty!");
    }

    void push(T val){
        Node<T>* newNode=new Node<T>(val);
        if(empty()){
            frontNode=newNode;
            newNode->next=nullptr;
            tail=newNode;
        }
        else{
            tail->next=newNode;
            tail=newNode;
        }

    }

    void pop(){
        Node<T>* temp=frontNode;
        frontNode=frontNode->next;
        delete temp;
        if (frontNode == nullptr) tail = nullptr;
    }


};

void printGanttChart(vector<int> order, vector<int> finTime) {
    cout << "\n Gantt Chart: \n";

    for (int i = 0; i < order.size(); i++) {
        cout << "------";
    }
    cout << "-\n";

    for (int i = 0; i < order.size(); i++) {
        cout << "| P" << order[i] << " ";
    }
    cout << "|\n";

    for (int i = 0; i < order.size(); i++) {
        cout << "------";
    }
    cout << "-\n";

    cout << "0";
    for (int i = 0; i < finTime.size(); i++) {
        cout << setw(6) << finTime[i];
    }
    cout << "\n\n";
}


//Turn Around Time=Completion Time - Arrival Time
//Waiting Time=Turn Around time - Burst Time


//First Come First Serve
void FCFS(vector<process> v){
    vector<int> order;
vector<int> finishTime;
queue<process> q;
int WT,TAT;
int CT=0;

for(int i=0;i<v.size();i++){
    q.push(v[i]);
}

 cout << left << setw(10) << "PID"
         << setw(15) << "Completion"
         << setw(15) << "TurnAround"
         << setw(15) << "Waiting" << endl;

while(!q.empty()){
    process p=q.front();
    q.pop();

    CT+=p.burst_time;
    TAT=CT-p.arrival_time;
    WT=TAT-p.burst_time;

     cout << left << setw(10) << p.id
             << setw(15) << CT
             << setw(15) << TAT
             << setw(15) << WT << endl;

    order.push_back(p.id);
    finishTime.push_back(CT);

}
printGanttChart(order, finishTime);

}

template<typename T,typename compare>
struct Heaps
{
   T arr[100];
   int size=0;
   compare comp;


   void insertion(T val){
    size++;
    arr[size]=val;

    int i = size;
    while(i > 1){
        int parent = i / 2;
        if(comp(arr[i], arr[parent])){
            swap(arr[i], arr[parent]);
            i = parent;
        }
        else break;
    }
   }

   void print(){
    for(int i=1;i<=size;i++){
        cout<<arr[i]<<"  ";
    }
   }

   void Deletion(){
     
    arr[1]=arr[size];
    size--;

    int i=1;
    while(i<size){
        int l=2*i;
        int r=2*i+1;

        if(l<=size && comp(arr[l],arr[i])){
            swap(arr[i],arr[l]);
            i=l;
        }
        else if(r<=size && comp(arr[r],arr[i])){
            swap(arr[r],arr[i]);
            i=r;
        }
        else{
        return;
        }
    }
   }

       void Heapify(int v[], int n, int i) {
        int largest = i;
        int left = 2 * i;
        int right = 2 * i + 1;

        if (left <= n && comp(v[left] , v[largest])) {
            largest = left;
        }
        if (right <= n && comp(v[right], v[largest])) {
            largest = right;
        }

        if (largest != i) {
            swap(v[i], v[largest]);
            Heapify(v, n, largest);
        }
    }

    void Heapsort(int v[], int n) {
        for (int i = n/2; i >= 1; i--) {
            Heapify(v, n, i);
        }

        int size = n;
        while (size > 1) {
            swap(v[1], v[size]);   
            size--;            
            Heapify(v, size, 1);  
        }
    }

    bool empty(){
        return size==0;
    }

};

struct CompareBurst {
    bool operator()(const process& a, const process& b) {
        return a.burst_time < b.burst_time;  
    }
};

void SJF(vector<process> v,int n){
    int CT,WT,TAT;
    CT=0;
    int completed=0;

    Heaps<process,CompareBurst> h;

    vector<bool> visited(n,false);
    int i=0;

      cout << left << setw(10) << "PID"
         << setw(15) << "Completion"
         << setw(15) << "TurnAround"
         << setw(15) << "Waiting" << endl;
     
    //assuming sorted process vector
    while(completed < n){

        while(i<n && v[i].arrival_time<=CT && !visited[i]){
            h.insertion(v[i]);
            visited[i]=true;
            i++;
        }


        if(!h.empty()){
        process f=h.arr[1];
        h.Deletion();

        CT+=f.burst_time;
        TAT=CT-f.arrival_time;
        WT=TAT-f.burst_time;

        cout << left << setw(10) << f.id
                 << setw(15) << CT
                 << setw(15) << TAT
                 << setw(15) << WT << endl;

        completed++;

    }
}
}

void RR(vector<process> v, int quantum) {
    int n = v.size();
    vector<int> rem_bt(n);   
    vector<int> CT(n);     
    int t = 0;             

    for (int i = 0; i < n; i++) {
        rem_bt[i] = v[i].burst_time;
    }

    queue<int> q; 
    vector<bool> inQueue(n, false);

    for (int i = 0; i < n; i++) {
        if (v[i].arrival_time <= t) {
            q.push(i);
            inQueue[i] = true;
        }
    }

    cout << left << setw(10) << "PID"
         << setw(15) << "Completion"
         << setw(15) << "TurnAround"
         << setw(15) << "Waiting" << endl;

    while (!q.empty()) {
        int i = q.front();
        q.pop();

        if (t < v[i].arrival_time) {
            t = v[i].arrival_time;
        }

        if (rem_bt[i] > quantum) {

            t += quantum;
            rem_bt[i] -= quantum;
        } else {
            t += rem_bt[i];
            rem_bt[i] = 0;
            CT[i] = t; 
        }

        for (int j = 0; j < n; j++) {
            if (!inQueue[j] && v[j].arrival_time <= t && rem_bt[j] > 0) {
                q.push(j);
                inQueue[j] = true;
            }
        }

        if (rem_bt[i] > 0) {
            q.push(i);
        }
    }

    for (int i = 0; i < n; i++) {
        int TAT = CT[i] - v[i].arrival_time;
        int WT = TAT - v[i].burst_time;
        cout << left << setw(10) << v[i].id
             << setw(15) << CT[i]
             << setw(15) << TAT
             << setw(15) << WT << endl;
    }
}



int main() {
    vector<process> v = {
        {1, 0, 5, 0},
        {2, 1, 3, 0},
        {3, 2, 8, 0},
        {4, 3, 6, 0}
    };

    int quantum=3;

    FCFS(v);

    return 0;
}

