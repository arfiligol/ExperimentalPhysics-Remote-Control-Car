import numpy as np

Vin = 7.8
Vout = 5
while True:
    R2 = float(input("Input the value of R2: "))
    R1 = R2 * (Vin - Vout) / Vout
    print("R1 need to be: " + str(R1))