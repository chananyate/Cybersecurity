class Node:
    def __init__(self, data=None):
        self.data = data
        self.next = None

class LinkedList:
    def __init__(self):
        self.head = None
        self.size = 0

    # When the first node is inserted, self.head is set to the new node, making it the first node in the list.
    # current is then initialized to self.head, giving it access to the Node's attributes (like next).
    def insert_at_end(self, data):
        new_node = Node(data)
        if self.head is None:
            self.head = new_node
        else:
            current = self.head
            while current.next:
                current = current.next
            current.next = new_node
        self.size += 1

    def remove_front(self):
        if self.head is None:
            return
        temp = self.head
        self.head = temp.next
        del temp
        self.size -= 1

    def get_first(self):
        return None if self.head is None else self.head.data

    def count_nodes(self):
        return self.size

    def is_empty(self):
        return self.head is None

# Creates an instance of LinkedList
ll = LinkedList()
print("Is the list empty?", ll.is_empty())

ll.insert_at_end(10)
ll.insert_at_end(20)
ll.insert_at_end(30)

print("Is the list empty?", ll.is_empty())
print("First element:", ll.get_first())
print("Number of nodes:", ll.count_nodes())
# Removes the first element
ll.remove_front()
print("First element after removal:", ll.get_first())
print("Number of nodes after removal:", ll.count_nodes())
