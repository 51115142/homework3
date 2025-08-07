#include <iostream>
#include <cmath>  
using namespace std;


struct Node {
    int coef;   // 係數
    int exp;    // 指數
    Node* link; // 指向下一個節點

    Node(int c = 0, int e = 0, Node* l = nullptr) : coef(c), exp(e), link(l) {}
};

class Polynomial {
private:
    Node* head;  // 頭節點（header node）

    // 清除所有節點
    void clear() {
        Node* curr = head->link;
        while (curr != head) {
            Node* temp = curr;
            curr = curr->link;
            delete temp;
        }
        head->link = head;
    }

    // 從另一個多項式 (Polynomial) 複製節點內容
    void copy(const Polynomial& other) {
        Node* tail = head;
        for (Node* p = other.head->link; p != other.head; p = p->link) {
            tail->link = new Node(p->coef, p->exp, head);
            tail = tail->link;
        }
    }

public:
    // 建構子：初始化循環鏈結串列
    Polynomial() {
        head = new Node();
        head->link = head;
    }

    // 複製建構子
    Polynomial(const Polynomial& other) {
        head = new Node();
        head->link = head;
        copy(other);
    }

    // 釋放記憶體
    ~Polynomial() {
        clear();
        delete head;
    }

    // 避免自我指派並複製節點
    Polynomial& operator=(const Polynomial& other) {
        if (this != &other) {
            clear();
            copy(other);
        }
        return *this;
    }

    // 輸入多項式（ n c1 e1 c2 e2 ...）
    friend istream& operator>>(istream& in, Polynomial& x) {
        int n;
        in >> n;           // 讀入項數
        x.clear();         // 清除原有內容
        Node* tail = x.head;
        for (int i = 0; i < n; i++) {
            int c, e;
            in >> c >> e;  // 每一項的係數與指數
            tail->link = new Node(c, e, x.head);
            tail = tail->link;
        }
        return in;
    }

    // 輸出多項式（例如：2x^3 - 4x + 5）
    friend ostream& operator<<(ostream& out, const Polynomial& x) {
        Node* p = x.head->link;
        bool first = true;
        while (p != x.head) {
            if (!first && p->coef >= 0)
                out << " + ";

            // 格式化輸出
            if (p->exp == 0)
                out << p->coef;
            else if (p->exp == 1)
                out << p->coef << "x";
            else
                out << p->coef << "x^" << p->exp;

            first = false;
            p = p->link;
        }
        if (first) out << "0"; // 空多項式
        return out;
    }

    // 多項式加法
    Polynomial operator+(const Polynomial& b) const {
        Polynomial result;
        Node* aPtr = head->link;
        Node* bPtr = b.head->link;
        Node* tail = result.head;

        while (aPtr != head && bPtr != b.head) {
            if (aPtr->exp > bPtr->exp) {
                tail->link = new Node(aPtr->coef, aPtr->exp, result.head);
                aPtr = aPtr->link;
            } else if (aPtr->exp < bPtr->exp) {
                tail->link = new Node(bPtr->coef, bPtr->exp, result.head);
                bPtr = bPtr->link;
            } else {
                int sum = aPtr->coef + bPtr->coef;
                if (sum != 0)
                    tail->link = new Node(sum, aPtr->exp, result.head);
                aPtr = aPtr->link;
                bPtr = bPtr->link;
            }
            if (tail->link) tail = tail->link;
        }

        // 剩下的項目（其中一方還沒走完）
        while (aPtr != head) {
            tail->link = new Node(aPtr->coef, aPtr->exp, result.head);
            tail = tail->link;
            aPtr = aPtr->link;
        }
        while (bPtr != b.head) {
            tail->link = new Node(bPtr->coef, bPtr->exp, result.head);
            tail = tail->link;
            bPtr = bPtr->link;
        }

        return result;
    }

    // 多項式減法（this - b）
    Polynomial operator-(const Polynomial& b) const {
        Polynomial negB;
        Node* tail = negB.head;
        for (Node* p = b.head->link; p != b.head; p = p->link) {
            tail->link = new Node(-p->coef, p->exp, negB.head);
            tail = tail->link;
        }
        return *this + negB;
    }

    // 多項式乘法
    Polynomial operator*(const Polynomial& b) const {
        Polynomial result;
        for (Node* aPtr = head->link; aPtr != head; aPtr = aPtr->link) {
            Polynomial temp;
            Node* tail = temp.head;
            for (Node* bPtr = b.head->link; bPtr != b.head; bPtr = bPtr->link) {
                int c = aPtr->coef * bPtr->coef;
                int e = aPtr->exp + bPtr->exp;
                tail->link = new Node(c, e, temp.head);
                tail = tail->link;
            }
            result = result + temp;  // 累加每個乘積
        }
        return result;
    }

    // 多項式數值求值：f(x)
    float Evaluate(float x) const {
        float result = 0;
        for (Node* p = head->link; p != head; p = p->link) {
            result += p->coef * pow(x, p->exp);
        }
        return result;
    }
};

int main() {
    Polynomial a, b;

    cout << "輸入第一個多項式 (n c1 e1 c2 e2 ...): ";
    cin >> a;
    cout << "輸入第二個多項式 (n c1 e1 c2 e2 ...): ";
    cin >> b;

    cout << "\na(x) = " << a << endl;
    cout << "b(x) = " << b << endl;

    cout << "\na + b = " << (a + b) << endl;
    cout << "a - b = " << (a - b) << endl;
    cout << "a * b = " << (a * b) << endl;

    float x = 2;
    cout << "\na(" << x << ") = " << a.Evaluate(x) << endl;

    return 0;
}
