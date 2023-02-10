#include <iostream>

class ListNode {
public:
  int val;
  ListNode* next;
  ListNode():val(0),next(nullptr) {}
  ListNode(int value):val(value),next(nullptr) {}
  ListNode(const ListNode &ln):val(ln.val),next(ln.next) {}
};

class LinkedList {
public:
  ListNode* head;
  
  LinkedList():head(nullptr) {}
  
  void insert(int value, int position) {
    ListNode* new_node = new ListNode(value);
    if (position == 0) {
      new_node->next = head;
      head = new_node;
    }
    ListNode* cur_node = head;
    int count = 0;
    while (cur_node != nullptr && count < position-1) {
      cur_node = cur_node->next;
      count++;
    }

    if (cur_node == nullptr) {
      std::cout << "Invalid position" << std::endl;
      delete new_node;
      return;
    }

    ListNode* next_node = cur_node->next;
    cur_node->next = new_node;
    new_node->next = next_node;
  }

  void print() {
    ListNode *tmp = head;
    while (tmp != nullptr) {
      std::cout << tmp->val << " -> ";
      tmp = tmp->next;
    }
    std::cout << "NULL" << std::endl;
  }

  ~LinkedList() {
    while (head != nullptr) {
      ListNode* tmp = head;
      head = head->next;
      delete tmp;
    }
  }
};

int main() {
  LinkedList ll;
  ll.insert(1,0);
  ll.insert(2,1);
  ll.insert(3,1);
  ll.print();
  return 0;
}