import unittest

class BankAccount:
    def __init__(self, id):
        self.id = id
        self.balance = 0

    def withdraw(self, amount):
        if self.balance >= amount:
            self.balance -= amount
            return True
        return False

    def deposit(self, amount):
        self.balance += amount
        return True

class TestBankAccount(unittest.TestCase):

    def setUp(self):
        self.account = BankAccount(id = 1)  # Sets up a bank account instance before each test.

    def test_multiple_transactions(self):
        # Tests a series of deposits and withdrawals.
        self.assertEqual(self.account.balance, 0)
        self.account.deposit(200)
        self.assertEqual(self.account.balance, 200)
        self.account.withdraw(50)
        self.assertEqual(self.account.balance, 150)
        self.account.deposit(100)
        self.assertEqual(self.account.balance, 250)
        self.account.withdraw(300)
        self.assertFalse(self.account.withdraw(300))
        self.assertEqual(self.account.balance, 250)

if __name__ == '__main__':
    unittest.main()

