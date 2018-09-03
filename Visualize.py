import matplotlib.pyplot as plt
import sys

if int(sys.argv[1]) == 0:
	x = []
	with open("out.txt", 'r') as fobj:
		for line in fobj:
			x.append([int(num) for num in line.split()])

			
	plt.plot(x[0],x[1], 'g')
	plt.xlabel('Time')
	plt.ylabel('Process ID')
	plt.show()
elif int(sys.argv[1]) == 1:
	x = []
	with open("out.txt", 'r') as fobj:
		for line in fobj:
			x.append([int(num) for num in line.split()])
			
	plt.plot(x[0],x[1], 'g')
	plt.plot(x[2],x[3], 'r')
	plt.plot(x[4],x[5], 'y')
	plt.xlabel('Time')
	plt.ylabel('Process ID')
	plt.title('Green = Round Robin; Red = FCFS; Yellow = SRTF')
	plt.show()


