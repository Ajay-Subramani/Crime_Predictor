import matplotlib.pyplot as plt
import sys
from collections import defaultdict

def parse_input(args):
    area_data = defaultdict(lambda: defaultdict(int))
    i = 1
    while i < len(args):
        area_name = args[i]
        i += 1
        while i < len(args) and args[i] in {"Morning", "Noon", "Evening", "Night", "Midnight"}:
            period = args[i]
            count = int(args[i + 1])
            area_data[area_name][period] = count
            i += 2
    return area_data

def plot_crime_time_analysis(area_data):
    areas = list(area_data.keys())
    time_periods = ['Morning', 'Noon', 'Evening', 'Night', 'Midnight']

    # Prepare data for each time period across all areas
    time_data = {period: [area_data[area].get(period, 0) for area in areas] for period in time_periods}

    # Plotting
    plt.figure(figsize=(12, 8))
    for period, counts in time_data.items():
        plt.plot(areas, counts, label=f"{period}", marker='o')

    # Labels and title
    plt.xlabel('Area')
    plt.ylabel('Number of Crimes')
    plt.title('Crime Time Analysis by Area')
    plt.xticks(rotation=45)
    plt.legend(loc='upper right')
    plt.tight_layout()

    highest_crime = 0
    highest_area = ""
    highest_period = ""
    for area, periods in area_data.items():
        for period, count in periods.items():
            if count > highest_crime:
                highest_crime = count
                highest_area = area
                highest_period = period

    # Display the area with the highest crime rate under the plot
    plt.figtext(0.5, -0.1, f"Highest Crime Rate: {highest_crime} crimes in {highest_area} during {highest_period}",
                ha="center", fontsize=12, color="red")
    # Display the plot
    plt.show()

if __name__ == "__main__":
    # Parse command-line arguments to create area_data dictionary
    area_data = parse_input(sys.argv)
    # Plot and analyze data
    plot_crime_time_analysis(area_data)



