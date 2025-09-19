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

//Turn Around Time=Completion Time - Arrival Time
//Waiting Time=Turn Around time - Burst Time


//First Come First Serve
void FCFS(vector<process> v){
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

}

}

struct cmp {
    bool operator()(process const& a, process const& b) {
        return a.burst_time > b.burst_time; 
    }
};

//Shortest job first algo 
void SJF(vector<process> v) {
    int n = v.size();
    int completed = 0, CT = 0;
    int WT, TAT;

    priority_queue<process, vector<process>, cmp> pq;

    vector<bool> inQueue(n, false);

    cout << left << setw(10) << "PID"
         << setw(15) << "Completion"
         << setw(15) << "TurnAround"
         << setw(15) << "Waiting" << endl;

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (!inQueue[i] && v[i].arrival_time <= CT) {
                pq.push(v[i]);

                inQueue[i] = true;
            }
        }

        if (!pq.empty()) 
        {
            process p = pq.top(); pq.pop();

            if (CT < p.arrival_time) CT = p.arrival_time;

            CT += p.burst_time;
            TAT = CT - p.arrival_time;
            WT = TAT - p.burst_time;

            cout << left << setw(10) << p.id
        << setw(15) << CT
                 << setw(15) << TAT
                 << setw(15) << WT << endl;

            completed++;
        } 
        else 
        {
            CT++;
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

    RR(v,quantum);

    return 0;
}

