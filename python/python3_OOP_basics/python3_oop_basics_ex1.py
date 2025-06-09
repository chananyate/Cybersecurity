import math

class Point:

    def __init__(self, x=0.0, y=0.0):
        if not isinstance(x, (int, float)) or not isinstance(y, (int, float)):  # Checks if x and y are numbers
            print("Error: x and y must be numbers.")
        else:
            self.x = x
            self.y = y

    def distance_from_origin(self):
         return math.sqrt(self.x**2 + self.y**2)  # Calculates the distance from the origin

p = Point(3, 5)

print(p.x, p.y)

p_invalid = Point(1, "two")  # Checks for invalid input

print(p.distance_from_origin())