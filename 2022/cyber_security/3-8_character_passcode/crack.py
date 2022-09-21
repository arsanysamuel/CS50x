from string import ascii_letters, digits, punctuation
from itertools import product  # cartesian product, equivalent to a nested for-loop, product('ABCD', repeat=2) = AA AB AC AD BA BB BC BD CA CB CC CD DA DB DC DD

# Crack 4-digit passcode
for passcode in product(ascii_letters + digits + punctuation, repeat=8):  # will take hundreds of years
    print(*passcode)  # astrisk for unpacking the list (removing quotes and brackets in printing)