import random


def generate_test_mass(n = 10, lower_bound=1, upper_bound=100):
    """Generate a list of n random integers between lower_bound and upper_bound."""
    return [random.randint(lower_bound, upper_bound) for _ in range(n)]

if __name__ == "__main__":
    test_mass = generate_test_mass(500, 0, 100)
    
    
    with open("tests\\size_test_mass.txt", "w") as file:
        file.write(len(test_mass).__str__())
        
    with open("tests\\random_test_mass.txt", "w") as file:
        file.write("\n".join(map(str, test_mass)))
        
    with open("tests\\sorted_test_mass.txt","w") as file:
        sorted_mass = sorted(test_mass)
        print(sorted_mass)
        file.write("\n".join(map(str, sorted_mass)))
        
    with open("tests\\reversed_test_mass.txt","w") as file:
        file.write("\n".join(map(str, sorted(test_mass, reverse=True))))
        
    with open("tests\\repeated_test_mass.txt","w") as file:
        repeated_mass = generate_test_mass(500, 0, 25)
        print(repeated_mass)
        file.write("\n".join(map(str, repeated_mass)))
        
        
    print("Test mass generated and saved to test_mass.txt")
    print(test_mass)