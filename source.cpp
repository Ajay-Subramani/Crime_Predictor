#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <cstdlib>
using namespace std;

// Structure to hold crime data
struct CrimeData {
    int year;
    string area;
    string crime_type;
    int population;
    double crime_rate;
    string time; // Time in 24-hour format
};

// List of crime data
vector<CrimeData> crimeDataset;

// Function to add crime data
void addCrimeData(int year, string area, string crime_type, int population, double crime_rate, string time) {
    CrimeData data = {year, area, crime_type, population, crime_rate, time};
    crimeDataset.push_back(data);
}

// Function to display crime data
void displayCrimeData() {
    cout << "Year\tArea\t\tCrime Type\tPopulation\tCrime Rate\tTime\n";
    for (const auto& data : crimeDataset) {
        cout << data.year << "\t" << setw(15) << data.area << "\t" 
             << setw(10) << data.crime_type << "\t" 
             << data.population << "\t\t" << fixed << setprecision(2) 
             << data.crime_rate << "\t\t" << data.time << endl;
    }
}

// Function to retrieve crime data for a specific year and count occurrences by crime type
map<string, int> getCrimeDataForYear(int year) {
    map<string, int> crimeCount;
    for (const auto& data : crimeDataset) {
        if (data.year == year) {
            crimeCount[data.crime_type]++;
        }
    }
    return crimeCount;
}

// Function to predict the overall crime rate based on linear regression
double predictCrimeRate(int year, int population) {
    if (crimeDataset.empty()) {
        cout << "No data available for prediction." << endl;
        return -1;
    }

    double sumX = 0, sumY = 0, sumXY = 0, sumXX = 0;
    int n = crimeDataset.size();

    for (const auto& data : crimeDataset) {
        double x = data.population;
        double y = data.crime_rate;

        sumX += x;
        sumY += y;
        sumXY += x * y;
        sumXX += x * x;
    }

    double slope = (n * sumXY - sumX * sumY) / (n * sumXX - sumX * sumX);
    double intercept = (sumY - slope * sumX) / n;

    double predictedRate = slope * population + intercept;
    return predictedRate;
}

// Predict crime rates for each type of crime based on population
map<string, double> predictCrimeDistribution(int population) {
    map<string, double> predictedCrimeRates;

    for (const auto& data : crimeDataset) {
        predictedCrimeRates[data.crime_type] += predictCrimeRate(data.year, population);
    }

    return predictedCrimeRates;
}

// Function to display crime rate pie charts by year and population prediction
void crimeRatePieChart() {
    int population;
    cout << "Enter population for future crime rate prediction: ";
    cin >> population;
    double predictedRate = predictCrimeRate(0, population);
                if (predictedRate != -1) {
                    cout << "Predicted Crime Rate: " << fixed << setprecision(2) << predictedRate << endl;
                } else {
                    cout << "Error in prediction." << endl;
                }
    int year;
    cout << "\nEnter the year for crime rate analysis: ";
    cin >> year;

    // Retrieve crime data for the specified year
    map<string, int> crimeDataForYear = getCrimeDataForYear(year);

    // Retrieve predicted crime distribution based on user-specified populati
    map<string, double> predictedCrimeRates = predictCrimeDistribution(population);

    // Prepare command for Python script, including year and crime data details
    stringstream command;
    command << "python C:\\Users\\Ajay\\OneDrive\\Desktop\\predictor\\crime_rate_pie_chart.py";
    
    // Append actual crime data for the specified year
    command << " " << year;
    for (const auto& crime : crimeDataForYear) {
        command << " " << crime.first << " " << crime.second;
    }

    // Append predicted crime rates
    command << " future";
    for (const auto& predicted : predictedCrimeRates) {
        command << " " << predicted.first << " " << fixed << setprecision(2) << predicted.second;
    }

    // Execute the Python script
    int result = system(command.str().c_str());

    if (result != 0) {
        cout << "Error: Python script did not execute successfully." << endl;
    }
}

// Function to upload and parse CSV data
bool uploadCSVData(const string& filename) {
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cout << "Could not open the file! Please check the file path." << endl;
        return false;
    }

    // Skip header
    getline(file, line);

    // Read CSV data and add to crimeDataset
    while (getline(file, line)) {
        stringstream ss(line);
        string yearStr, area, crime_type, populationStr, crime_rateStr, time;

        if (!getline(ss, yearStr, ',') || 
            !getline(ss, area, ',') || 
            !getline(ss, crime_type, ',') || 
            !getline(ss, populationStr, ',') || 
            !getline(ss, crime_rateStr, ',') || 
            !getline(ss, time, ',')) {
            cout << "Skipping line due to missing fields: " << line << endl;
            continue;
        }

        try {
            int year = stoi(yearStr);
            int population = stoi(populationStr);
            double crime_rate = stod(crime_rateStr);
            
            addCrimeData(year, area, crime_type, population, crime_rate, time);
        } catch (const invalid_argument& e) {
            cout << "Error: Non-numeric data in numeric field on line: " << line << endl;
            continue;
        } catch (const out_of_range& e) {
            cout << "Error: Numeric value out of range on line: " << line << endl;
            continue;
        }
    }

    file.close();
    return true;
}

// Function to simulate crime time analysis by time period
string getTimePeriod(int hour) {
    if (hour >= 6 && hour < 9) return "Morning";
    else if (hour >= 9 && hour < 12) return "Noon";
    else if (hour >= 12 && hour < 18) return "Evening";
    else if (hour >= 18 && hour < 21) return "Night";
    else return "Midnight";
}

// Function to analyze crime counts by time period and area
void crimeTimeAnalysis() {
    map<string, map<string, int>> crimeCountByTimePeriod;

    for (const auto& data : crimeDataset) {
        int hour = stoi(data.time.substr(0, 2)); // Extract hour from 24-hour format
        string timePeriod = getTimePeriod(hour);

        crimeCountByTimePeriod[data.area][timePeriod]++;
    }

    // Prepare command for Python script
    stringstream command;
    command << "python C:\\Users\\Ajay\\OneDrive\\Desktop\\predictor\\crime_time_analysis.py";

    for (const auto& areaData : crimeCountByTimePeriod) {
        command << " " << areaData.first;
        for (const auto& periodData : areaData.second) {
            command << " " << periodData.first << " " << periodData.second;
        }
    }

    // Execute the command
    int result = system(command.str().c_str());
    if (result != 0) {
        cout << "Error: Python script for time analysis did not execute successfully." << endl;
    }
}

// Main program menu
int main() {
    int choice;
    bool exit = false;

    while (!exit) {
        cout << "\n1. Add Crime Data\n2. Display Crime Data\n3. Predict Crime Rate\n4. Crime Time Analysis\n";
        cout << "5. Upload and Analyze CSV Data\n6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int year, population;
                string area, crime_type, time;
                double crime_rate;

                cout << "Enter Year: ";
                cin >> year;
                cout << "Enter Area: ";
                cin >> area;
                cout << "Enter Crime Type: ";
                cin >> crime_type;
                cout << "Enter Population: ";
                cin >> population;
                cout << "Enter Crime Rate: ";
                cin >> crime_rate;
                cout << "Enter Time (24-hour format HH:MM): ";
                cin >> time;

                addCrimeData(year, area, crime_type, population, crime_rate, time);
                break;
            }

            case 2:
                displayCrimeData();
                break;

            case 3: {
                crimeRatePieChart();
                break;
            }

            case 4:
                crimeTimeAnalysis();
                break;

            case 5: {
                string filename;
                cout << "Enter the path of the CSV file: ";
                cin >> filename;

                if (uploadCSVData(filename)) {
                    cout << "Data uploaded and analyzed successfully!" << endl;
                } else {
                    cout << "Failed to upload or analyze data." << endl;
                }
                break;
            }

            case 6:
                exit = true;
                break;

            default:
                cout << "Invalid choice!" << endl;
        }
    }

    return 0;
}


