from random import randint

if __name__ == "__main__":
    n = 10
    with open("input", 'w') as f:
        f.write(f"{n}\n")
        max_value = 1000000000
        for i in range(n):
            f.write(f"{randint(-max_value, max_value)} ")
