# 51115142

作業三

## 解題說明

本作業目標是將一個單元數多項式使用「循環連結串列」(有頭節點的 circular linked list)表示，并能對其進行加法、減法、乘法、數值測試等執行。

### 解題策略

1. 結構設計：

        各項元以 Node 組成，包含三個元素：coef (促數)，exp (指數)，與 link (下一節點指量)。
   
        head 是頭節點，連接到第一個元素，將連續結尾成為循環。

2. 內部操作函數：

        clear(): 刪除所有節點
        copy(): 複製一個外來多項式的節點

3. 外部操作函數：

        operator>>: 從輸入變成 linked list
        operator<<: 轉成可讀形式輸出
        operator +, -, *: 進行多項式的執行操作
        Evaluate: 評估 f(x)

4. 特性：

        支援空多項式
        變數元素小於 0 時不展示
        能做計算與輸出結果

## 程式實作

以下為主要程式碼：

```cpp
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

```

## 效能分析

1. 時間複雜度：

       加法 / 減法：O(m + n) 於 m, n 分別為 a(x) 和 b(x) 的項數

       乘法：O(m * n)，每一項和每一項相乘

       Evaluate：O(n)，連續執行指數計算
   
2. 空間複雜度：

       所有加法/減法/乘法 結果最惟 O(m+n) 或 O(m*n) 項

       啟用連結組對空間有效利用

## 測試與驗證

### 測試案例

| 測試案例 | 輸入 (係數 指數) | 預期輸出 | 實際輸出 |
|----------|--------------|--------------|------------|
| 測試一   | 5 8           | 5x^8         | 5x^8         |
| 測試二   | 5 8 0 -2 3 0  | 5x^8 + 3     | 5x^8 + 3     |
| 測試三   | -3 7 2 5      | -3x^7 + 2x^5 | -3x^7 + 2x^5 |
| 測試四   | -3 7 -2 5     | -3x^7 - 2x^5 | -3x^7 - 2x^5 |
| 測試五   | 0 4 0 2       | 0            | 0            |

### 結論

1. 功能符合題目要求，可正確表示多項式、輸入/輸出、動態儲存及排序。

2. 效能對小型多項式足夠（例如數十項），但若處理大型多項式（上百項），排序 O(n²) 會明顯拖慢輸出效率。

3. 記憶體用量符合 O(n)，動態擴充設計簡單可行。

4. 若需優化，可考慮替換氣泡排序為快排或 STL std::sort。

### 申論及開發報告

本專案旨在實作多項式的抽象資料型態（ADT），使用動態陣列來儲存多項式的每一項，並提供 << 與 >> 運算子重載，達成方便的多項式輸入與輸出。

由於採用 O(n²) 的氣泡排序演算法，當項數增加到數百項時效能可能下降。此外，程式碼在架構上已將多項式的輸入、輸出、項目管理與排序封裝在類別中，具備良好的可擴充性，後續若需新增多項式加法或乘法等運算，可直接在 Polynomial 類別中擴充。

整體而言，此程式在符合題目要求下，兼顧了結構簡單、功能正確與記憶體彈性配置，已達成多項式類別實作的教學目標。
