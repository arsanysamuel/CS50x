# Credit pset in python
import re


def main():
    # Getting user input
    num_regex = re.compile("\d*")
    try:
        num = input("Number: ")
        i = int(num)
    except ValueError:
        pass

    # Compiling regex validators
    visa = re.compile("^4(\d{12}|\d{15})$")
    mc = re.compile("^(5[1-5])+\d{14}$")
    amex = re.compile("^(34|37)+\d{13}$")

    # Checking validity
    if amex.match(num) and check_validity(num):
        print("AMEX")

    elif visa.match(num) and check_validity(num):
        print("VISA")

    elif mc.match(num) and check_validity(num):
        print("MASTERCARD")

    else:
        print("INVALID")


def check_validity(card_number):
    '''
    Using Luhn’s Algorithm to check the validity of the card number
    '''
    number = card_number[::-1]
    every_other = [int(number[i]) * 2 for i in range(1, len(number), 2)]
    total = sum([n if n // 10 == 0 else (n // 10 + n % 10) for n in every_other])
    total += sum([int(number[i]) for i in range(0, len(number), 2)])
    return True if total % 10 == 0 else False


if __name__ == "__main__":
    main()