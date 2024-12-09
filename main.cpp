#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Window.H>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

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
Fl_Text_Buffer *displayBuffer = new Fl_Text_Buffer();

// Function to add crime data
void addCrimeData(int year, string area, string crime_type, int population, double crime_rate, string time) {
    CrimeData data = {year, area, crime_type, population, crime_rate, time};
    crimeDataset.push_back(data);
}

// Function to display crime data
void displayCrimeData() {
    ostringstream oss;
    oss << "Year\tArea\t\tCrime Type\tPopulation\tCrime Rate\tTime\n";
    for (const auto& data : crimeDataset) {
        oss << data.year << "\t" << setw(15) << data.area << "\t" 
            << setw(10) << data.crime_type << "\t" 
            << data.population << "\t\t" << fixed << setprecision(2) 
            << data.crime_rate << "\t\t" << data.time << endl;
    }
    displayBuffer->text(oss.str().c_str());
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

// Function to predict crime rates for each type of crime based on population
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

    // Retrieve predicted crime distribution based on user-specified population
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

// Callback for Add Crime Data Window
void addCrimeDataCallback(Fl_Widget *widget, void *data) {
    Fl_Input **inputs = (Fl_Input **)data;
    int year = stoi(inputs[0]->value());
    string area = inputs[1]->value();
    string crimeType = inputs[2]->value();
    int population = stoi(inputs[3]->value());
    double crimeRate = stod(inputs[4]->value());
    string time = inputs[5]->value();

    // Add data to crimeDataset
    addCrimeData(year, area, crimeType, population, crimeRate, time);

    // Clear input fields
    for (int i = 0; i < 6; ++i) {
        inputs[i]->value("");
    }

    // Close the window
    widget->window()->hide();
}

// Callback for Upload CSV
void uploadCSVCallback(Fl_Widget *widget, void *data) {
    const char *filename = fl_file_chooser("Choose a CSV file", "*.csv", nullptr);
    if (filename) {
        if (uploadCSVData(filename)) {
            displayCrimeData(); // Display the updated crime data
        } else {
            displayBuffer->text("Failed to upload CSV data.\n");
        }
    }
}

// Create the main window for adding crime data
void createAddCrimeDataWindow() {
    Fl_Window *addWindow = new Fl_Window(400, 400, "Add Crime Data");

    Fl_Input *yearInput = new Fl_Input(100, 50, 200, 30, "Year");
    Fl_Input *areaInput = new Fl_Input(100, 90, 200, 30, "Area");
    Fl_Input *crimeTypeInput = new Fl_Input(100, 130, 200, 30, "Crime Type");
    Fl_Input *populationInput = new Fl_Input(100, 170, 200, 30, "Population");
    Fl_Input *crimeRateInput = new Fl_Input(100, 210, 200, 30, "Crime Rate");
    Fl_Input *timeInput = new Fl_Input(100, 250, 200, 30, "Time");

    Fl_Input *inputs[] = {yearInput, areaInput, crimeTypeInput, populationInput, crimeRateInput, timeInput};

    Fl_Button *addButton = new Fl_Button(100, 300, 100, 30, "Add Data");
    addButton->callback(addCrimeDataCallback, (void *)inputs);

    Fl_Button *cancelButton = new Fl_Button(210, 300, 100, 30, "Cancel");
    cancelButton->callback([](Fl_Widget* widget, void*) { widget->window()->hide(); });

    addWindow->end();
    addWindow->show();
}

// Create the main window
void createMainWindow() {
    Fl_Window *mainWindow = new Fl_Window(600, 500, "Crime Data Analysis");

    Fl_Button *addCrimeDataButton = new Fl_Button(50, 50, 200, 30, "Add Crime Data");
    addCrimeDataButton->callback([](Fl_Widget*, void*) { createAddCrimeDataWindow(); });

    Fl_Button *uploadCSVButton = new Fl_Button(50, 100, 200, 30, "Upload CSV");
    uploadCSVButton->callback(uploadCSVCallback, nullptr);

    Fl_Button *displayButton = new Fl_Button(50, 150, 200, 30, "Display Crime Data");
    displayButton->callback([](Fl_Widget*, void*) { displayCrimeData(); });

    Fl_Button *predictButton = new Fl_Button(50, 200, 200, 30, "Predict Crime Rate");
    predictButton->callback([](Fl_Widget*, void*) { crimeRatePieChart(); });

    Fl_Button *timeAnalysisButton = new Fl_Button(50, 250, 200, 30, "Crime Time Analysis");
    timeAnalysisButton->callback([](Fl_Widget*, void*) { crimeTimeAnalysis(); });

    Fl_Text_Display *display = new Fl_Text_Display(300, 50, 250, 400);
    display->buffer(displayBuffer);

    mainWindow->end();
    mainWindow->show();
}

int main() {
    createMainWindow();
    return Fl::run();
}

