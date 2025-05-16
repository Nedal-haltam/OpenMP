#include <iostream>
#include <time.h>
#include <vector>
#include <sstream>
#include <ios>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <iomanip>


/*
TODO: 
    - report for sizes: 500, 1000, 5000, and 10000
    - Modify Version 2 to solve the race condition. Show that your code now generates correct results.
*/
clock_t Clocker;
void StartClock()
{
    Clocker = clock();
}
void EvaluateClock()
{
    clock_t t = clock() - Clocker;
    std::cout << "Time taken: " << (double)(t) / CLOCKS_PER_SEC << "s\n";
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
    int LocalSize = vec.size() / ThreadCount;
    int start = ThreadID * LocalSize;
    int end = (ThreadID + 1) * LocalSize;
    for (int i = start; i < end; i++) {
        sum += vec[i];
    }
    return sum / vec.size();
}
#endif

#define ARRAY_LEN(arr) (sizeof(arr) / sizeof(arr[0]))
int main(int argc, char* argv[]) {
    std::cout << std::setprecision(10);
    if (argc < 2)
    {
        std::cout << "Size is not provided\n";
        return 1;
    }
    int size = strtol(argv[1], NULL, 10);
    std::vector<double> arr = std::vector<double>(size);
    
    for (size_t i = 0; i < arr.size(); i++)
    {
        arr[i] = i;
    }
#ifdef _OPENMP
    double Average = 0;
    double correct = GetAverageSerial(arr);
#ifdef RACE_COND
    do
    {
        Average = 0;
        StartClock();
        #pragma omp parallel num_threads(12)
        {
            double avg = GetAverageParallel(arr);
            Average += avg;
        }
    } while((float)correct == (float)Average);
    EvaluateClock();
    std::cout << "INFO: Calculated average of size: " << arr.size() << " and the average is: " << Average << "\n";
    std::cout << "INFO: The correct average of size: " << arr.size() << " is: " << correct << "\n";
#else
    do
    {
        Average = 0;
        StartClock();
        #pragma omp parallel num_threads(12)
        {
            double avg = GetAverageParallel(arr);
            #pragma omp critical
            Average += avg;
        }
    } while(0);
    EvaluateClock();
    // } while((float)correct == (float)Average);
    std::cout << "INFO: Calculated average of size: " << arr.size() << " and the average is: " << Average << "\n";
    std::cout << "INFO: The correct average of size: " << arr.size() << " is: " << correct << "\n";
#endif

#else
    StartClock();
    double Average = GetAverageSerial(arr);
    EvaluateClock();
    std::cout << "INFO: Calculated average of size: " << arr.size() << " and the average is: " << Average << "\n";
#endif
    return 0;
}
