import sys
import matplotlib.pyplot as plt

def plot_pie_chart(data, title):
    labels = list(data.keys())
    sizes = list(data.values())
    plt.figure(figsize=(6, 6))
    plt.pie(sizes, labels=labels, autopct='%1.1f%%', startangle=140)
    plt.title(title)
    plt.show()

if __name__ == "__main__":
    # Check if there are enough arguments
    if len(sys.argv) < 3:
        print("Error: Not enough arguments provided to the script.")
        print("Usage: python crime_rate_pie_chart.py <year> <crime_type1> <count1> ... future <crime_typeX> <predicted_countX>")
        sys.exit(1)

    args = sys.argv[1:]

    # Extract the actual crime data for the year
    year = args[0]
    actual_data = {}
    index = 1
    while args[index] != "future":
        crime_type = args[index]
        count = int(args[index + 1])
        actual_data[crime_type] = count
        index += 2

    # Extract predicted crime data
    predicted_data = {}
    index += 1  # Skip "future" keyword
    while index < len(args):
        crime_type = args[index]
        predicted_rate = float(args[index + 1])
        predicted_data[crime_type] = predicted_rate
        index += 2

    # Plot actual data pie chart
    plot_pie_chart(actual_data, f"Crime Rate Distribution for {year}")

    # Plot predicted data pie chart
    plot_pie_chart(predicted_data, "Predicted Crime Rate Distribution")
