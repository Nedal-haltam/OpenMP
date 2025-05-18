import subprocess
import matplotlib.pyplot as plt
import os

def Execute(args : list[str], verbose : bool = False) -> subprocess.CompletedProcess[str]:
    Result = subprocess.run(args, capture_output=True, text=True)
    if verbose:
        if Result.returncode == 0:
            print(Result.stdout)
        else:
            print(f'failed to execute {Result.args}, exit code is {Result.returncode}', end='\n\n')
    return Result



Directory = os.getcwd() + r'\Generated'

SerialPath = Directory + r'\serial.exe'
RacePath = Directory + r'\race.exe'
ParallelPath = Directory + r'\parallel.exe'

BufferSize = '20000000'
NumberOfThreads = '10'


ParallelExecutionTimes : list[float] = []
print(f'Executing Serially')
SerialExecutionTime : float = float(Execute([SerialPath, BufferSize]).stdout)

for i in range(1, 13):
    time = 0
    while time == 0:
        print(f'Executing on {i} Threads')
        time = float(Execute([ParallelPath, BufferSize, i.__str__()]).stdout)
    ParallelExecutionTimes.append(time)

print(f'Serial Execution Time : {SerialExecutionTime}')
print(f'Parallel Execution Times : {ParallelExecutionTimes}')

SpeedUp : list[float] = [int(SerialExecutionTime / x * 100000) / 100000.0 for x in ParallelExecutionTimes]
print(f'Speed Up for each Parallel Execution Times : {SpeedUp}')



plt.figure(figsize=(8, 5))
plt.plot(range(1, 13), SpeedUp, marker='o')
plt.title('Speed Up vs Number of Threads')
plt.xlabel('Number of Threads')
plt.ylabel('Speed Up')
plt.grid(True)
plt.savefig('Speed-Up.png')


print(end='\n\n')
print(f'Executing Serially')
SerialExecutionTime : float = float(Execute([SerialPath, BufferSize]).stdout)
print(f'Executing Parallel on 120 Threads')
Parallel10times = float(Execute([ParallelPath, BufferSize, '120']).stdout)

print(f'Serial Execution time: {SerialExecutionTime}')
print(f'Parallel Execution time on 120: {Parallel10times}')
print(f'Speed Up of 120 Threads over serial is: {SerialExecutionTime / Parallel10times}')