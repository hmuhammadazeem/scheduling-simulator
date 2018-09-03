#pragma once
#include <queue>
#include <vector>
#include <Windows.h>
#include <iostream>
using namespace std;


class PCB {
public:
	int pid;
	int arrivalTime;
	int burstTime;
	PCB *next;
	int originalBurst;

	PCB();
	PCB(int a, int b, int c);
};

class CircularQueue {
public:
	PCB * head;
	PCB * tail;

	void push(PCB *proc);
	PCB* pop();

	CircularQueue();
	~CircularQueue() { while (head != 0) pop(); }

};

class Scheduler {
public:
	int timeQuantum;
	Scheduler(double a) : timeQuantum(a) { readyQueue = new CircularQueue(); };
	CircularQueue * readyQueue;
	
	vector<int> x;
	vector<int> y;
	
	void RoundRob(queue<PCB> &input);
	void FCFS(queue<PCB> &input);
	void SRTF(queue<PCB> &input);
};