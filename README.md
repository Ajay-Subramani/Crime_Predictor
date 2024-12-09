# Crime_Predictor

Crime Rate Analysis Tool

  This project is a Crime Data Analysis Tool that allows users to analyze crime data, predict crime rates for specific populations, and generate graphs and charts based on uploaded or entered crime data. The tool is built using C++ for the backend and utilizes the FLTK (Fast Light Toolkit) framework for creating the graphical user interface (GUI). Python scripts are integrated into the project to generate graphical outputs (such as pie charts) for crime rate predictions and crime time analysis.

Features

Add Crime Data: Allows users to input crime data manually (e.g., year, area, crime type, population, crime rate, and time).
Display Crime Data: Displays all crime data entries in a structured table format.
Crime Rate Prediction: Predicts crime rates for a specific population using linear regression.
Crime Time Analysis: Analyzes and displays the distribution of crimes based on time of day.
Upload and Parse CSV Data: Users can upload a CSV file containing crime data, which is then parsed and added to the tool's dataset.
Generate Crime Rate Graphs: Utilizes Python to generate pie charts and graphs based on crime rate predictions and actual data.
Handle External Python Scripts: Integrates Python scripts that generate visualizations for crime rate prediction and time-based analysis.
Prerequisites
Before you can run the Crime Rate Analysis Tool, ensure that you have the following installed:

Software Requirements:
C++ Compiler:

GCC or any C++ compiler that supports C++11 or higher.
Ensure that you have FlTK installed.
Python (for running the external scripts):

Python 3.x (Ensure matplotlib and numpy libraries are installed for graphing).
Install Python libraries using:
bash
Copy code
pip install matplotlib numpy
FLTK Library:

Install FLTK for C++ GUI support.
You can download it from FLTK official website.
Text Editor/IDE:

Any text editor or IDE of your choice (e.g., Visual Studio Code, CLion, or Eclipse) for development.
Project Structure
bash
Copy code
Crime-Rate-Analysis-Tool/
├── src/                       # C++ source code
│   ├── main.cpp               # Main entry point for C++ application
│   ├── crime_data.cpp         # Functions for crime data handling
│   ├── crime_data.h           # Header file for crime data structures and functions
├── python/                    # Python scripts for generating graphs
│   ├── crime_rate_pie_chart.py # Python script to generate pie chart for crime rate
│   ├── crime_time_analysis.py # Python script for crime time analysis
├── data/                      # Directory to store example CSV files
│   ├── crime_data.csv         # Example CSV file with crime data
├── README.md                  # This README file
├── Makefile                   # Makefile for building the C++ program
└── LICENSE                    # License information
Installation and Setup
1. Clone the Repository
Clone this repository to your local machine using Git:

bash
Copy code
git clone https://github.com/your-username/Crime-Rate-Analysis-Tool.git
2. Install FLTK Library
Download and install FLTK from fltk.org, or install it via your package manager. For example:

Ubuntu/Debian:

bash
Copy code
sudo apt-get install libfltk1.3-dev
macOS (via Homebrew):

bash
Copy code
brew install fltk
3. Compile the C++ Program
Navigate to the src/ directory and compile the C++ program using the provided Makefile (or any other build system you prefer).

bash
Copy code
cd src/
make
This will generate the executable for the Crime Rate Analysis Tool.

4. Install Python Dependencies
Ensure you have Python 3.x installed, and then install the necessary libraries:

bash
Copy code
pip install matplotlib numpy
5. Configure Python Scripts
Ensure that the Python scripts crime_rate_pie_chart.py and crime_time_analysis.py are in the python/ directory. These scripts are responsible for generating crime-related graphs and visualizations. Ensure they are working by running them manually first.

Usage
1. Run the C++ Program
Once everything is set up, navigate to the directory where the executable is located and run the program:

bash
Copy code
./CrimeRateAnalysisTool
This will launch the Crime Rate Analysis Tool with a graphical user interface (GUI).

2. Using the Application
The main window of the tool will allow you to perform the following operations:

Add Crime Data: Input new crime data manually (e.g., year, area, crime type, population, crime rate, and time). After entering the data, click Add Data to add it to the internal dataset.
Display Crime Data: View the entered crime data in a table format within the GUI.
Upload CSV Data: Upload a CSV file with crime data. The tool will parse the file and display the data accordingly.
Predict Crime Rate: Input a population to predict the crime rate for that population based on the existing dataset using linear regression.
Crime Time Analysis: Display a breakdown of crime occurrences by time of day (e.g., Morning, Noon, Evening, Night).
Crime Rate Graphs: Generate and view crime rate pie charts and graphs using the integrated Python scripts.
3. Python Script Integration
Crime Rate Pie Chart: This Python script generates a pie chart based on the predicted crime rates for a given population. It is called from the C++ program when the Crime Rate Prediction feature is used.

Crime Time Analysis: This Python script generates a graph showing the distribution of crime occurrences based on different times of the day. It is invoked when the Crime Time Analysis feature is used.

Example CSV Format
The CSV file should contain the following columns:

csv
Copy code
year,area,crime_type,population,crime_rate,time
2023,Area1,Burglary,50000,5.2,14:30
2023,Area2,Robbery,100000,8.3,22:15
...
year: The year of the crime occurrence.
area: The geographical area where the crime occurred.
crime_type: The type of crime (e.g., Burglary, Robbery).
population: The population of the area.
crime_rate: The crime rate for that type of crime.
time: The time of day when the crime occurred (in 24-hour format, e.g., 14:30).
Sample CSV File:
csv
Copy code
year,area,crime_type,population,crime_rate,time
2023,Downtown,Burglary,50000,6.5,12:15
2023,Suburbs,Robbery,150000,4.1,16:30
2023,Park,Assault,20000,7.8,22:00
Contributions
Feel free to fork this repository, make improvements, and submit pull requests. Contributions are always welcome!

License
This project is licensed under the MIT License - see the LICENSE file for details.

Acknowledgments
FLTK: For providing the framework to develop the GUI in C++.
Python: For its powerful libraries like matplotlib and numpy for generating graphs and visualizations.
Open Source Community: For making the tools and libraries available that make this project possible.
