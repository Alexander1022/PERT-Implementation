#include <iostream>
#include "Chart.h"

int main()
{
    Chart *chart = new Chart(6);
    // The default file name is "input.txt"
    chart->readFromFile();
    chart->pertChart(0);

    Chart* chart2 = new Chart(7);
    chart2->readFromFile("input2.txt");
    chart2->pertChart(0);

    Chart* chart3 = new Chart(8);
    chart3->readFromFile("input2.txt");
    chart3->pertChart(0);

    return 0;
}