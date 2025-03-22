//Объект- студент поля: ФИО, группа, номер зачетной книжки, массив 4-х оценок за сессию; Сортировка по ФИО; Исходный контейнер deque коприруем в list.
#include <iostream>
#include <fstream>
#include <deque>
#include <list>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

class Student {
public:
    string full_name;
    string group;
    int record_book_number;
    int grades[4];

    Student(string name, string grp, int book_num, int grds[4]) {
        full_name = name;
        group = grp;
        record_book_number = book_num;
        for (int i = 0; i < 4; ++i) {
            grades[i] = grds[i];
        }
    }

    Student(const Student& other) {
        full_name = other.full_name;
        group = other.group;
        record_book_number = other.record_book_number;
        for (int i = 0; i < 4; ++i) {
            grades[i] = other.grades[i];
        }
    }

    Student& operator=(const Student& other) {
        if (this != &other) {
            full_name = other.full_name;
            group = other.group;
            record_book_number = other.record_book_number;
            for (int i = 0; i < 4; ++i) {
                grades[i] = other.grades[i];
            }
        }
        return *this;
    }

    friend ostream& operator<<(ostream& os, const Student& student);
    bool operator<(const Student& other) const {
        return full_name < other.full_name;
    }
    bool operator==(const Student& other) const {
        return full_name == other.full_name && 
               group == other.group &&
               record_book_number == other.record_book_number;
    }
};

ostream& operator<<(ostream& os, const Student& student) {
    os << "ФИО: " << student.full_name << ", Группа: " << student.group
       << ", Номер зачетной книжки: " << student.record_book_number
       << ", Оценки: ";
    for (int grade : student.grades) {
        os << grade << " ";
    }
    return os;
}

struct StudentHash {
    size_t operator()(const Student& student) const {
        size_t h1 = hash<string>()(student.full_name);
        size_t h2 = hash<string>()(student.group);
        size_t h3 = hash<int>()(student.record_book_number);
        return h1 ^ (h2 << 1) ^ (h3 << 2); // Комбинируем хэши
    }
};

int main() {
    ifstream input("input.txt");
    if (!input.is_open()) {
        cerr << "Файл не открылся!" << endl;
        return 1;
    }

    deque<Student> students_deque;

    string full_name, group;
    int record_book_number;
    int grades[4];
    while (input >> full_name >> group >> record_book_number >> grades[0] >> grades[1] >> grades[2] >> grades[3]) {
        students_deque.push_back(Student(full_name, group, record_book_number, grades));
    }
    input.close();

    list<Student> students_list;
    copy(students_deque.begin(), students_deque.end(), back_inserter(students_list));

    students_list.sort();

    set<Student> students_set(students_deque.begin(), students_deque.end());
    unordered_set<Student, StudentHash> students_unordered_set(students_deque.begin(), students_deque.end());

    // Копируем unordered_set в вектор для сортировки
    vector<Student> students_vector(students_unordered_set.begin(), students_unordered_set.end());
    sort(students_vector.begin(), students_vector.end());

    ofstream output("output.txt");
    if (!output.is_open()) {
        cerr << "Файл не открылся" << endl;
        return 1;
    }

    output << "Отсортированный список (list):" << endl;
    for (const auto& student : students_list) {
        output << student << endl;
    }

    output << "\nКонтейнер set (автоматически отсортирован):" << endl;
    for (const auto& student : students_set) {
        output << student << endl;
    }

    output << "\nКонтейнер unordered_set (отсортирован после копирования в вектор):" << endl;
    for (const auto& student : students_vector) {
        output << student << endl;
    }

    output.close();
    cout << "Данные записаны" << endl;
    return 0;
}