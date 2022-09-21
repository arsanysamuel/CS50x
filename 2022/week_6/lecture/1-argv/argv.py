# Command line arguments
import sys

if len(sys.argv) == 2:
    print(f"Hello, {argv[1]}")
    sys.exit(0)
else:
    print("Missing cmd line arg")
    sys.exit(1)  # Can exit with exit codes with sys.exit()