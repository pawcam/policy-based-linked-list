#include "LinkedList.h"
#include "ut.hpp"
#include <cstring>

using namespace boost::ut;

suite<"LinkedListSuite"> LinkedListSuite = [] {
  "LinkedList"_test = [] {
    LinkedList<int> list;
    expect(!list.head());
    expect(!list.tail());
    expect(list.empty());
  };

  "LinkedList_Int"_test = [] {
    std::vector<int> app_data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    LinkedList<int> list;
    std::for_each(app_data.begin(), app_data.end(), [&](int& i) {
      list.insert_back(i);
    });

    expect(list.size() == app_data.size());
    auto foundNode = list.find(app_data[4]);
    if (foundNode)
    {
      expect(foundNode->m_data == 5);
    }
    else
    {
      expect(false) << "Node not found";
    }

    list.remove(app_data[4]);
    expect(list.size() == app_data.size() - 1);
    foundNode = list.find(app_data[4]);
    expect(!foundNode);
  };


  static constexpr auto MAX_NAME_LEN = 20;
  static constexpr auto MAX_DESC_LEN = 40;
  struct Widget
  {
    uint32_t id{0};
    char name[20]        = {0};
    char description[40] = {0};

    Widget& operator=(const Widget& rhs)
    {
      id = rhs.id;
      std::strncpy(name, rhs.name, MAX_NAME_LEN);
      std::strncpy(name, rhs.description, MAX_DESC_LEN);

      return *this;
    }

    bool operator==(const Widget& rhs) const
    {
      return id == rhs.id &&
             std::strncmp(name, rhs.name, MAX_NAME_LEN) == 0 &&
             std::strncmp(description, rhs.description, MAX_DESC_LEN) == 0;
    }
  };

  "WidgetTest"_test = []
  {
    Widget w1{1, "Widget1", "Description1"};
    Widget w2{2, "Widget2", "Description2"};
    Widget w3{3, "Widget3", "Description3"};

    LinkedList<Widget> list;
    list.insert_back(w1);
    list.insert_front(w2);
    list.insert_back(w3);

    expect(list.size() == 3);
    expect(list.head()->m_data == w2);
    expect(list.tail()->m_data == w3);
    expect(list.getPrev(list.find(w1))->m_data == w2);
    expect(list.getNext(list.find(w2))->m_data == w1);
    expect(list.getNext(list.find(w1))->m_data == w3);
    expect(list.getPrev(list.find(w3))->m_data == w1);

    expect(list.remove(w2));
    expect(list.size() == 2);
    expect(list.head()->m_data == w1);
    expect(list.tail()->m_data == w3);
    expect(!list.find(w2));

    expect(list.remove(w3));
    expect(list.size() == 1);
    expect(list.head()->m_data == w1);
    expect(list.tail()->m_data == w1);
    expect(list.head() == list.tail());
    expect(!list.find(w3));

    expect(list.remove(w1));
    expect(list.size() == 0);
    expect(list.empty());
    expect(!list.find(w1));
  };
};

int main()
{
  return 0;
}