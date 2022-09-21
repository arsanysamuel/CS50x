# Readability in python
import re


def main():
    # Getting user input
    text = input("Text: ")

    # Calculating words and letters
    words = len([word for word in text.split()])
    letters = len([l for l in text if l.isalpha()])
    sentences = len([s for s in re.split(r"\.|\!|\?", text) if len(s) >= 2])

    # Coleman-Liau formula
    L_var = letters / words * 100
    S_var = sentences / words * 100
    grade = round(0.0588 * L_var - 0.296 * S_var - 15.8)

    # Printing results
    if grade > 16:
        print("Grade 16+")
    elif grade < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {grade}")


if __name__ == "__main__":
    main()
