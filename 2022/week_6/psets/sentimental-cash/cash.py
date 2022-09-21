# Cash pset in python
owed = 0

# Getting user input
while owed <= 0:
    try:
        owed = float(input("Change owed: "))
    except ValueError:
        pass

# Calculating coins
change = int(owed * 100)
coins = 0

quarters = change // 25
change -= quarters * 25
coins += quarters

dimes = change // 10
change -= dimes * 10
coins += dimes

nickels = change // 5
change -= nickels * 5
coins += nickels

cents = change // 1
change -= cents * 1
coins += cents

# Printing coins
print(coins)
