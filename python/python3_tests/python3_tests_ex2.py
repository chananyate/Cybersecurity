import pytest

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

"""@pytest.fixture
def account():
    # Fixture to create a new BankAccount instance before each test. 
    # Since I only have one test it's not needed here.
    # We need a fixture when multiple tests require the same setup or need isolation between tests.
    return BankAccount(id = 1)"""

def test_multiple_transactions():
    # Tests a series of deposits and withdrawals
    # The balance is initially 0 by default
    account = BankAccount(id = 1)
    assert account.balance == 0
    account.deposit(200)
    assert account.balance == 200
    account.withdraw(50)
    assert account.balance == 150
    account.deposit(100)
    assert account.balance == 250
    assert not account.withdraw(300)
    assert account.balance == 250
