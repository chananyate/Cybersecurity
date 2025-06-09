import csv
import statistics

data = []
min_age_vaccinated = float('inf')
max_age_vaccinated = -float('inf')
min_age_unvaccinated = float('inf')
max_age_unvaccinated = -float('inf')
hos_length = []

with open('corona.csv', 'r') as csvfile:
    reader = csv.reader(csvfile)
    next(reader)  # Skips header row
    for row in reader:
        data.append(row)
        age = int(row[1])
        hos_length.append(int(row[4]))

        # Determines min and max ages for vaccinated and unvaccinated
        if row[6] == 'Y':  # Vaccinated
            min_age_vaccinated = min(min_age_vaccinated, age)
            max_age_vaccinated = max(max_age_vaccinated, age)
        elif row[6] == 'N':  # Unvaccinated
            min_age_unvaccinated = min(min_age_unvaccinated, age)
            max_age_unvaccinated = max(max_age_unvaccinated, age)

    # Calculates average hospitalization length outside the loop
    average_hos = statistics.mean(hos_length)

    print("Minimum age (vaccinated):", min_age_vaccinated)
    print("Maximum age (vaccinated):", max_age_vaccinated)
    print("Minimum age (unvaccinated):", min_age_unvaccinated)
    print("Maximum age (unvaccinated):", max_age_unvaccinated)
    print("The average hospitalization length is:", average_hos)

def filter_func():
    filter_type = input("What data do you need? gender, age_range, vaccinated: ").lower()
    filtered_data = []
    if filter_type == "gender":
        gender = input("Enter gender: M/F: ")
        filtered_data = [row for row in data if row[0] == gender]
    elif filter_type == "age_range":
        minimum_age = int(input("Enter minimum age: "))
        maximum_age = int(input("Enter maximum age: "))
        filtered_data = [row for row in data if minimum_age <= int(row[1]) <= maximum_age]
    elif filter_type == "vaccinated":
        vaccinated_status = input("Enter vaccination status: Y/N: ")
        filtered_data = [row for row in data if row[6] == vaccinated_status]

    if filtered_data:
        output_filename = input("Enter the name of the file to save the filtered data (like filtered.csv): ")
        with open(output_filename, 'w') as file:
            writer = csv.writer(file)
            writer.writerow(['Gender', 'Age', 'Ventilated', 'Time between positive and hospitalization', 'Hospitalization Length', 'Time between positive and recovery', 'Vaccinated'])
            writer.writerows(filtered_data)
            print(f"Filtered data saved to {output_filename}")
    else:
        print("No data matches the given filter.")

filter_func()
