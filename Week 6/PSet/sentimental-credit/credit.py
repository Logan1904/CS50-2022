from cs50 import get_string

number = get_string("Number: ")
n = len(number)

sum = 0
for i in range(n-2,-1,-2):
    to_add = int(number[i])*2
    if len(str(to_add)) >= 2:
        for i in range(len(str(to_add))):
            sum += int(str(to_add)[i])
    else:
        sum += to_add

for i in range(n-1,-1,-2):
    sum += int(number[i])

if str(sum)[-1] == "0":
    if (n == 13 or n == 16) and number[0] == "4":
        print("VISA")
    elif (n == 16) and (number[0] == "5") and (number[1] == "1" or number[1] == "2" or number[1] == "3" or number[1] == "4" or number[1] == "5"):
        print("MASTERCARD")
    elif (n == 15) and (number[0] == "3") and (number[1] == "4" or number[1] == "7"):
        print("AMEX")
    else:
        print("INVALID")
else:
    print("INVALID")
