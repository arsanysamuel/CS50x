from string import digits  # Literally the string '012345689'
from itertools import product  # cartesian product, equivalent to a nested for-loop, product('ABCD', repeat=2) = AA AB AC AD BA BB BC BD CA CB CC CD DA DB DC DD

# Crack 4-digit passcode
for passcode in product(digits, repeat=4):
    print(*passcode)  # astrisk for unpacking the list (removing quotes and brackets in printing)
    pass