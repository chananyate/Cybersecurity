def money_change(amount):
    two_hundred_bill = one_hundred_bill = fifty_bill = twenty_bill = 0
    ten_shekel = five_shekel = shnekel = shekel = sum = 0

    while amount > 0:
        two_hundred_bill = amount // 200  # floor division
        amount = amount - (two_hundred_bill * 200)
        sum += two_hundred_bill

        one_hundred_bill = amount // 100
        amount = amount - (one_hundred_bill * 100)
        sum += one_hundred_bill

        fifty_bill = amount // 50
        amount = amount - (fifty_bill * 50)
        sum += fifty_bill

        twenty_bill = amount // 20
        amount = amount - (twenty_bill * 20)
        sum += twenty_bill

        ten_shekel = amount // 10
        amount = amount - (ten_shekel * 10)
        sum += ten_shekel

        five_shekel = amount // 5
        amount = amount - (five_shekel * 5)
        sum += five_shekel

        shnekel = amount // 2
        amount = amount - (shnekel * 2)
        sum += shnekel

        shekel = amount // 1
        amount = amount - (shekel * 1)
        sum += shekel

    print("Number of 200 bills: ", two_hundred_bill)
    print("Number of 100 bills: ", one_hundred_bill)
    print("Number of 50 bills: ", fifty_bill)
    print("Number of 20 bills: ", twenty_bill)
    print("Number of 10 shekels coins: ", ten_shekel)
    print("Number of 5 shekels coins: ", five_shekel)
    print("Number of 2 shekels coins: ", shnekel)
    print("Number of 1 shekel coins: ", shekel)
    print("The minimum number of bills and coins is: ", sum)

amount = int(input("Enter an amount: "))
money_change(amount)
