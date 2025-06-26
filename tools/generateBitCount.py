#! /usr/bin/python3

"""
Generate a C/C++-style lookup table for an 8 bit number for counting the
number of bits set in the number.
"""

def count_bits_set(number: int) -> int:
    count = 0
    while number != 0:
        number &= number - 1
        count += 1
    return count


def main():
    print("uint8_t lookupTable[256] = {", end="")
    for i in range(2<<7):
        if i % 16 == 0:
            print("\n\t", end="")
        print(count_bits_set(i), end="")
        if i is not 255:
            print(",", end="")
        print(" ", end="")
    print("\n}\n", end="")


if __name__ == "__main__":
    main()