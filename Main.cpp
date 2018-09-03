#include <fstream>
#include "Scheduler.h"

void loadData(ifstream &data, queue<PCB> &input);

int main() {

	cout << "1. Round Robin\n2. FCFS\n3. SRTF\n4. Compare Graphically\n->";
	int n;
	cin >> n;

	ifstream data("input.txt");
	queue<PCB> input;
	loadData(data, input);
	int t = 0;
	ofstream output("out.txt");
	Scheduler *sim = new Scheduler(0);

	switch (n)
	{
	case 1:
		cout << "Time Quantum: "; cin >> t;
		sim->timeQuantum = t;
		sim->RoundRob(input);
		break;
	case 2:
		sim->FCFS(input);
		break;
	case 3:
		sim->SRTF(input);
		break;
	default:
		break;
	}

	

	if (n != 4) {
		ofstream _output("out.txt");
		for (int i = 0; i < sim->x.size(); i++)
			_output << sim->x[i] << " ";
		_output << "\n";

		for (int i = 0; i < sim->y.size(); i++)
			_output << sim->y[i] << " ";
		_output.close();
		system("python Visualize.py 0");
	}
	
	return 0;
}


void loadData(ifstream &data, queue<PCB> &input) {

	int pid, arrival, burst;
	PCB *load;
	int i = 0;
	while(!data.eof()) {
		data >> pid;
		data >> arrival;
		data >> burst;

		load = new PCB(pid, arrival, burst);
		input.push(*load);
		delete load;
	}

}