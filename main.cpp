#include <iostream>
#include <time.h>
#include <vector>
#include <sstream>
#include <ios>
#include <assert.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <iomanip>

char* ShiftArgs(int* argc, char*** argv)
{
    assert(*argc > 0);
    char* result = **argv;
    (*argv) += 1;
    (*argc) -= 1;
    return result;
}

clock_t Clocker;
void StartClock()
{
    Clocker = clock();
}
double EvaluateClock(bool Verbose = false)
{
    clock_t t = clock() - Clocker;
    double TimeTaken = (double)(t) / CLOCKS_PER_SEC;
    if (Verbose)
        std::cout << "Time taken: " << TimeTaken << "s\n";
    return TimeTaken;
}

double GetAverageSerial(std::vector<double>& vec)
{
    double average = 0.0f;
    for (size_t i = 0; i < vec.size(); i++)
    {
        average += vec[i];
    }
    average /= vec.size();
    return average;
}

#ifdef _OPENMP
double GetAverageParallel(std::vector<double>& vec)
{
    int ThreadID = omp_get_thread_num();
    int ThreadCount = omp_get_num_threads();

    double sum = 0.0;
    int SizeOverCount = vec.size() / ThreadCount;
    int start = ThreadID * SizeOverCount;
    int end = ((vec.size() <= ThreadCount || vec.size() % ThreadCount != 0) && ThreadID == ThreadCount - 1) ? vec.size() : (ThreadID + 1) * SizeOverCount;

    for (int i = start; i < end; i++) {
        sum += vec[i];
    }
    return sum / vec.size();
}
void ParallelExecution(int argc, char** argv)
{
    std::string ProgramName = ShiftArgs(&argc, &argv);
    double Average = 0;
    if (argc < 2)
    {
        std::cout << "Usage: " << ProgramName << "[options] <Buffer size> <Number of threads>\n";
        std::cout << "Options:\n";
        std::cout << "-v: print the time taken\n";
        exit(EXIT_FAILURE);
    }
    int Size = strtol(ShiftArgs(&argc, &argv), NULL, 10);
    if (Size == 0)
    {
        std::cout << "No average for Buffer size zero\n";
        exit(EXIT_FAILURE);
    }
    int NumberOfThreads = strtol(ShiftArgs(&argc, &argv), NULL, 10);
    bool Verbose = argc > 0 && ShiftArgs(&argc, &argv) == "-v";

    std::vector<double> arr = std::vector<double>(Size);
    for (size_t i = 0; i < arr.size(); i++)
    {
        arr[i] = i;
    }
    double correct = GetAverageSerial(arr);
#ifdef RACE_COND
    do
    {
        Average = 0;
        StartClock();
        #pragma omp parallel num_threads(NumberOfThreads)
        {
            double avg = GetAverageParallel(arr);
            Average += avg;
        }
    } while((float)correct == (float)Average);
    double TimeTaken = EvaluateClock();
    if (Verbose)
        std::cout << "INFO: Calculated average of size: " << arr.size() << " and the average is: " << Average << "\n";
    else
        std::cout << TimeTaken << "\n";
    if (Verbose)
        std::cout << "INFO: The correct average of size: " << arr.size() << " is: " << correct << "\n";
#else
    do
    {
        Average = 0;
        StartClock();
        #pragma omp parallel num_threads(NumberOfThreads)
        {
            double avg = GetAverageParallel(arr);
            #pragma omp critical
            Average += avg;
        }
    } while(0);
    double TimeTaken = EvaluateClock();
    // } while((float)correct == (float)Average);
    if (Verbose)
        std::cout << "INFO: Calculated average of size: " << arr.size() << " and the average is: " << Average << "\n";
    else
        std::cout << TimeTaken << "\n";
    if (Verbose)
        std::cout << "INFO: The correct average of size: " << arr.size() << " is: " << correct << "\n";
#endif
}
#endif

void SerialExecution(int argc, char** argv)
{
    std::string ProgramName = ShiftArgs(&argc, &argv);
    if (argc < 1)
    {
        std::cout << "Usage: " << ProgramName << " [options] <Buffer size>\n";
        std::cout << "Options:\n";
        std::cout << "-v: print the time taken\n";
        exit(EXIT_FAILURE);
    }
    int Size = strtol(ShiftArgs(&argc, &argv), NULL, 10);
    bool Verbose = argc > 0 && (std::string)ShiftArgs(&argc, &argv) == "-v";
    if (Size == 0)
    {
        std::cout << "No average for Buffer size zero\n";
        exit(EXIT_FAILURE);
    }
    
    std::vector<double> arr = std::vector<double>(Size);
    for (size_t i = 0; i < arr.size(); i++)
    {
        arr[i] = i;
    }

    StartClock();
    double Average = GetAverageSerial(arr);
    (void)Average;
    double TimeTaken = EvaluateClock();
    if (Verbose)
        std::cout << "INFO: Calculated average of size: " << arr.size() << " and the average is: " << Average << "\n";
    else
        std::cout << TimeTaken << "\n";
}

int main(int argc, char** argv) {
    std::cout << std::setprecision(10);
#ifdef _OPENMP
    ParallelExecution(argc, argv);
#else
    SerialExecution(argc, argv);
#endif
    return 0;
}
