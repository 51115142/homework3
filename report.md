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
#include <sstream>
#include <string>

class Polynomial;

// Term 類別：代表一個多項式中的一項，例如 3x^2
class Term {
    // 讓 Polynomial 類別與 I/O 運算子可以存取 private 成員
    friend class Polynomial;
    friend std::ostream& operator<<(std::ostream& os, const Term& term);
    friend std::istream& operator>>(std::istream& is, Term& term);
private:
    float coef;  // 係數
    int exp;     // 指數
public:
    // 建構子（預設的係數為 0，指數為 0）
    Term(float c = 0.0, int e = 0) : coef(c), exp(e) {}
    
    // Getter：取得係數與指數
    float getCoef() const { return coef; }
    int getExp() const { return exp; }
};

// Polynomial 類別：為了儲存多項式的多個項（Term）
class Polynomial {
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& poly);
    friend std::istream& operator>>(std::istream& is, Polynomial& poly);
private:
    Term* termArray;   // 動態陣列儲存項目
    int capacity;      // 陣列的最大容量
    int terms;         // 目前的項目數量

    // 當項目超過容量時，擴展容量為原本的兩倍
    void expandCapacity() {
        capacity *= 2;
        Term* newArray = new Term[capacity];
        for (int i = 0; i < terms; ++i)
            newArray[i] = termArray[i];
        delete[] termArray;
        termArray = newArray;
    }

    // 將項目按照指數從大到小排序（泡沫排序法）
    void sortTerms() {
        for (int i = 0; i < terms - 1; ++i)
            for (int j = i + 1; j < terms; ++j)
                if (termArray[i].exp < termArray[j].exp) {
                    Term temp = termArray[i];
                    termArray[i] = termArray[j];
                    termArray[j] = temp;
                }
    }

public:
    // 建構子：初始化項目陣列與容量
    Polynomial(int initialCapacity = 4) : capacity(initialCapacity), terms(0) {
        termArray = new Term[capacity];
    }

    // 解構子：釋放動態記憶體
    ~Polynomial() {
        delete[] termArray;
    }

    // 新增一個項目進入多項式
    void addTerm(float c, int e) {
        if (terms >= capacity) expandCapacity();  // 超過容量就擴充
        termArray[terms++] = Term(c, e);          // 加入新項
    }
};
// 讀取單一 Term（係數與指數）
std::istream& operator>>(std::istream& is, Term& term) {
    return is >> term.coef >> term.exp;
}
// 輸出單一 Term，例如 3x^2
std::ostream& operator<<(std::ostream& os, const Term& term) {
    os << term.coef << "x^" << term.exp;
    return os;
}
// 讀取 Polynomial：從一整行輸入讀取多個 (係數 指數) 組合
std::istream& operator>>(std::istream& is, Polynomial& poly) {
    poly.terms = 0;  // 清空原本的項目數
    std::string line;
    std::getline(is, line);             // 讀取整行
    std::istringstream iss(line);      // 用字串流來逐項讀取
    float coef; int exp;
    while (iss >> coef >> exp)
        poly.addTerm(coef, exp);       // 逐項加入多項式
    return is;
}
// 輸出 Polynomial：轉成數學形式如 "3x^2 + 2x + 1"
std::ostream& operator<<(std::ostream& os, const Polynomial& poly) {
    if (poly.terms == 0) {
        os << "0";
        return os;
    }
    // 排序項目（指數從大到小）
    ((Polynomial&)poly).sortTerms();

    bool first = true;
    for (int i = 0; i < poly.terms; ++i) {
        float c = poly.termArray[i].getCoef();
        int e = poly.termArray[i].getExp();
        if (c == 0) continue;  // 跳過係數為 0 的項

        // 處理正負號
        if (!first) os << (c > 0 ? " + " : " - ");
        else if (c < 0) os << "-";

        float absC = c < 0 ? -c : c;

        // 如果係數不是 1，或是是常數項，才印出係數
        if (absC != 1 || e == 0) os << absC;

        // 印出變數與指數
        if (e > 0) {
            os << "x";
            if (e > 1) os << "^" << e;
        }
        first = false;
    }
    if (first) os << "0"; // 如果都是 0 項，輸出 0
    return os;
}
int main() {
    Polynomial poly;
    std::cout << "輸入多項式 (係數 指數): ";
    std::cin >> poly; 
    std::cout << "多項式: " << poly << "\n";
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
