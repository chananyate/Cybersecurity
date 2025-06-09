import pytest
import time
import datetime

class Machine:
    def __init__(self, machine_type):
        self.machine_type = machine_type
        self.start_time = None  # Start_time is None until the machine is started
        self.running = False
        self.cost_per_minute = 2 if machine_type == 1 else 5

    def start_machine(self):
        if not self.running:  # Starts machine only if it's not already running
            self.start_time = datetime.datetime.now()
            self.running = True
            print(f"Machine of type {self.machine_type} started at {self.start_time}")
        else:
            print("Machine is already running.")

    def stop_machine(self):
        if self.running:
            stop_time = datetime.datetime.now()
            print(f"Machine stopped at {stop_time}.")
            print(f"Total cost for this session: ${self.get_current_cost():.2f}")
            self.running = False
        else:
            print("Machine is not running.")

    def get_current_cost(self):
        if self.running:
            elapsed_time = (datetime.datetime.now() - self.start_time).total_seconds() / 60
            return elapsed_time * self.cost_per_minute
        else:
            return 0


class CostTracker:
    def __init__(self):
        self.machines = {}  # Initializes an empty dictionary to store machine instances

    def add_machine(self, machine_id, machine):
        self.machines[machine_id] = machine  # Adds the machine instance to the dictionary with machine_id as the key and the machine instance as the value

    def remove_machine(self, machine_id):
        if machine_id in self.machines:
            self.machines[machine_id].stop_machine()  # Stops the machine before removing
            del self.machines[machine_id]  # Removes it from the dictionary

    def get_total_cost(self):  # Calculates total cost for all running machines
        total_cost = 0
        for machine in self.machines.values():
            total_cost += machine.get_current_cost()
        return total_cost


@pytest.fixture
def setup_machines():
    tracker = CostTracker()
    machine_1 = Machine(1)
    machine_2 = Machine(2)
    tracker.add_machine("machine_1", machine_1)
    tracker.add_machine("machine_2", machine_2)
    return tracker, machine_1, machine_2

def test_total_cost(setup_machines):
    tracker, machine_1, machine_2 = setup_machines

    machine_1.start_machine()
    machine_2.start_machine()

    time.sleep(6)
    # Checks the cost for both machines running for 6 seconds
    assert tracker.get_total_cost() == pytest.approx(0.7, abs=0.01)  # Allowing small error

    machine_1.stop_machine()
    # Checks the cost after stopping machine_1 (only machine_2 should contribute)
    assert tracker.get_total_cost() == pytest.approx(0.5, abs=0.01)  # Machine 2 is still running

    tracker.remove_machine("machine_2")

    # After removing machine_2, the total cost should be 0
    assert tracker.get_total_cost() == 0  # All machines are stopped and removed

    # Edge case: stop a machine that is already stopped
    machine_1.stop_machine()  # No effect since it's already stopped
    assert tracker.get_total_cost() == 0  # Cost should still be 0 after redundant stop

    # Edge case: remove a non-existent machine
    tracker.remove_machine("machine_3")  # Machine 3 doesn't exist, so no effect
    assert tracker.get_total_cost() == 0  # Cost should still be 0 after removal of non-existent machine