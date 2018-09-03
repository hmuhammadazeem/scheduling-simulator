#include "Scheduler.h"
#include <numeric>


PCB::PCB() {
	pid = -1;
	arrivalTime = -1;
	burstTime = -1;
	next = NULL;
	originalBurst = 0;
}

PCB::PCB(int a, int b, int c) : pid(a), arrivalTime(b), burstTime(c) { next = NULL;};

CircularQueue::CircularQueue() {
	head = NULL;
	tail = NULL;
}

void CircularQueue::push(PCB *proc) {
	PCB* temp = new PCB();
	temp->pid = proc->pid;
	temp->arrivalTime = proc->arrivalTime;
	temp->burstTime = proc->burstTime;
	temp->originalBurst = temp->burstTime;

	if (head == NULL) {
		head = temp;
	}
	else {
		tail->next = temp;
	}

	tail = temp;
	tail->next = head;
}

PCB* CircularQueue::pop() {
	if (head == NULL)
			return NULL;

	if (head == tail) {
		PCB* temp = new PCB();
		temp->pid = head->pid;
		temp->arrivalTime = head->arrivalTime;
		temp->burstTime = head->burstTime;
		delete head;
		head = NULL;
		tail = NULL;

	}
	else {
		PCB* c;
		PCB* temp = new PCB();
		temp->pid = head->next->pid;
		temp->arrivalTime = head->next->arrivalTime;
		temp->burstTime = head->next->burstTime;
		c = head;
		head = head->next;
		tail->next = head;
		delete c;
	}
}

void Scheduler::RoundRob(queue<PCB> &input) {

	bool differentArrivals = true;
	int timer = 0;
	bool fetched = false; //if a single process in ready queue
	bool removed = false; //if a process completed

	int idleTime = 0;
	vector<int> waitingTimes;
	vector<int> turnAround;
	vector<int> responseTime;


	do {

		while (!input.empty() && input.front().arrivalTime <= timer && !fetched) {
			readyQueue->push(&input.front());
			input.pop();
		}

		if (readyQueue->head && readyQueue->tail && readyQueue->head->next && 
			readyQueue->head->arrivalTime != readyQueue->head->next->arrivalTime && differentArrivals && !removed) {
				readyQueue->head = readyQueue->head->next;
				readyQueue->tail = readyQueue->tail->next;
		}
		else {
			removed = false;
			differentArrivals = true;
		}

		if (readyQueue->head) {
			if (readyQueue->head->burstTime == readyQueue->head->originalBurst) {
				responseTime.push_back(timer - readyQueue->head->arrivalTime);
			}
		}


		for (int i = 0; i < timeQuantum; i++) {
				x.push_back(timer);
				y.push_back(readyQueue->head->pid);
			if (!(readyQueue->head->burstTime > 0)) {
				cout << "Finished process: " << readyQueue->head->pid << endl;
				turnAround.push_back(timer - readyQueue->head->arrivalTime);
				waitingTimes.push_back((timer - readyQueue->head->arrivalTime) - readyQueue->head->originalBurst);
				readyQueue->pop();
				removed = true;
				break;
			}
			else if (readyQueue->head->burstTime > 0) {
				readyQueue->head->burstTime -= 1;
			}
			else {
				cout << "<Time: " << timer << "> Process running: IDLE" << endl;
				timer += 1;
				idleTime++;
			}
			Sleep(1);	
			cout << "<Time: " << timer << "> Process running: " << readyQueue->head->pid << endl;
			timer += 1;
		}
		

		if (readyQueue->head == readyQueue->tail) {
			while (!input.empty() && input.front().arrivalTime <= timer) {
				readyQueue->push(&input.front());
				input.pop();
				fetched = true;
			}
		}
		else
			fetched = false;

		if (readyQueue->head != NULL) {
			if (readyQueue->head->arrivalTime == readyQueue->head->next->arrivalTime && readyQueue->head != readyQueue->head->next) {
				readyQueue->head = readyQueue->head->next;
				readyQueue->tail = readyQueue->tail->next;
				differentArrivals = false;
			}
		}
		else {
			differentArrivals = true;
		}	
		
	} while (readyQueue->head != NULL || !input.empty());

	cout << "CPU Usage: " << (((timer - idleTime) / timer) * 100) << "%" << endl;
	cout << "Average Waiting Time: " << (accumulate(waitingTimes.begin(), waitingTimes.end(), 0) / waitingTimes.size()) << endl;
	cout << "Average Response Time: " << (accumulate(responseTime.begin(), responseTime.end(), 0) / responseTime.size()) << endl;
	cout << "Average Turnaround Time: " << (accumulate(turnAround.begin(), turnAround.end(), 0) / turnAround.size()) << endl;

}


void Scheduler::FCFS(queue<PCB> &input) { //input = input queue containing data from file
	
	int timer = 0;
	int idleTime = 0;
	vector<int> waitingTimes;
	vector<int> turnAround;
	vector<int> responseTime;

	do {

		while (!input.empty() && input.front().arrivalTime <= timer) {
			readyQueue->push(&input.front());
			input.pop();
		}

		if (readyQueue->head->burstTime == readyQueue->head->originalBurst) {
			responseTime.push_back(timer - readyQueue->head->arrivalTime);
		}
			if (!(readyQueue->head->burstTime > 0)) {
				cout << "<Time: " << timer << "> Finished process: " << readyQueue->head->pid << endl;
				turnAround.push_back(timer - readyQueue->head->arrivalTime);
				waitingTimes.push_back((timer - readyQueue->head->arrivalTime) - readyQueue->head->originalBurst);
				readyQueue->pop();
			}
			else if(readyQueue->head->burstTime > 0){
				readyQueue->head->burstTime -= 1;
				Sleep(1);
				x.push_back(timer);
				y.push_back(readyQueue->head->pid);
				cout << "<Time: " << timer << "> Process running: " << readyQueue->head->pid << endl;
				timer += 1;
			}
			else {
				cout << "<Time: " << timer << "> Process running: IDLE" << endl;
				timer += 1;
				idleTime++;
			}

	} while (readyQueue->head != NULL || !input.empty());

	cout << "CPU Usage: " << (((timer - idleTime) / timer) * 100) << "%" << endl;
	cout << "Average Waiting Time: " << (accumulate(waitingTimes.begin(), waitingTimes.end(), 0) / waitingTimes.size()) << endl;
	cout << "Average Response Time: " << (accumulate(responseTime.begin(), responseTime.end(), 0) / responseTime.size()) << endl;
	cout << "Average Turnaround Time: " << (accumulate(turnAround.begin(), turnAround.end(), 0) / turnAround.size()) << endl;

}

void Scheduler::SRTF(queue<PCB> &input) {

	vector<int> waitingTimes;
	vector<int> turnAround;
	vector<int> responseTime;

	int Finish;                                  //Finish timefor current process
	int Rem_Process = 0, total, cpu, sum_wait = 0, sum_turnaround = 0;
	double result;                               //Storing the Result of current process and for calculating averge
	int smallest;
	total = input.size();

	vector<int> pid;
	vector<int> defaultBursts;
	vector<int> arrival;
	vector<int> burst;
	vector<int> rem_time;
	
	for (int i = 0; i < total; i++)             //initializing the arrays
	{	
		arrival.push_back(input.front().arrivalTime);
		burst.push_back(input.front().burstTime);
		rem_time.push_back(burst[i]);
		defaultBursts.push_back(burst[i]);
		pid.push_back(input.front().pid);
		input.pop();
	}

	for (cpu = 0; Rem_Process != total; cpu++)   //Loop for number of processes
	{
		smallest = 0;
		for (int i = 0; i < total; i++)     // Loop for finding the index which have minimum arrival time and least remaing time
		{
			if (arrival[i] <= cpu && rem_time[i] < rem_time[smallest] && rem_time[i] > 0)
			{
				smallest = i;
			}
			if (burst[i] == defaultBursts[i])
				responseTime.push_back(cpu - arrival[i]);
		}
		rem_time[smallest]--;
		x.push_back(cpu);
		y.push_back(pid[smallest]);
		cout << "<Time: " << cpu << "> Process running: " << pid[smallest] << endl;
		if (!rem_time[smallest] > 0)     // current Process ending
		{
			//Rem_Process++;
			Finish = cpu + 1;
			result = Finish - arrival[smallest]; //calculating turn arround cpu
			turnAround.push_back(result);
			result = Finish - burst[smallest] - arrival[smallest];   // calculating burst cpu
			waitingTimes.push_back(result);
			arrival.erase(arrival.begin() + smallest);
			rem_time.erase(rem_time.begin() + smallest);
			pid.erase(pid.begin() + smallest);
			total--;
		}
	}

	cout << "Average Waiting Time: " << (accumulate(waitingTimes.begin(), waitingTimes.end(), 0) / waitingTimes.size()) << endl;
	cout << "Average Response Time: " << (accumulate(responseTime.begin(), responseTime.end(), 0) / responseTime.size()) << endl;
	cout << "Average Turnaround Time: " << (accumulate(turnAround.begin(), turnAround.end(), 0) / turnAround.size()) << endl;

}