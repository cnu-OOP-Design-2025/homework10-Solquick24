#pragma once

#include <initializer_list>
#include <cstddef>
#include <algorithm>


template<typename T>
class MyVector {
    T* data;            // 동적 배열을 가리키는 포인터
    size_t cap;         // 현재 할당된 배열 크기
    size_t length;      // 실제 저장된 요소 개수    
public:

    MyVector(std::initializer_list<T> init) {
        length = init.size();
        cap = length;
        data = new T[cap];
        size_t i = 0;
        for (const auto& val : init) {
            data[i++] = val;
        }
    }

    MyVector(): data(nullptr), length(0), cap(0) {}
    ~MyVector() { delete[] data; }

    void push_back(const T& val) {

        if ( length == cap ) {  // resize 연산

            size_t new_cap = (cap == 0)? 1: cap * 2;
            T* new_data = new T[new_cap];

            for ( size_t i = 0; i < length; ++i )  {
                new_data[i] = data[i];
            }

            delete[] data;
            data = new_data;
            cap = new_cap;
        }
        
        data[length++] = val;
    }

    void pop_back() {
        if ( length == 0 ) {
            throw std::out_of_range("pop_back from empty vector");
        }
        --length;
    }

    size_t size() const {
        return length;
    }           // Vecotr에 있는 항목 개수(length)를 리턴하는 함수 구현

    T& operator[](int i) {
        if ( i >= length ) {
            throw std::out_of_range("index out of range");
        }
        return data[i];
    }         // i 번째 항목에 접근하는 연산자 오버로딩 구현

    bool operator==(const MyVector& other) const { return length == other.length && std::equal(data, data + length, other.data); }
    // std::equal을 사용하려면 #include <algorithm> 필요

    bool operator!=(const MyVector& other) const { return !(*this == other); }


    // operator< 를 기본으로 서로 재귀호출 하지 않도록 구현
    bool operator<(const MyVector& other) const { 
        size_t n = (length < other.length)? length : other.length;
        // 두 벡터 중 짧은 쪽의 길이를 n. 
        // 비교할 요소는 공통 부분(둘 다 존재하는 인덱스들)까지만 하면 되기 때문
        
        for ( size_t i = 0; i < n; i++ ) { // 사전식 비교를 수동으로 구현
            if ( data[i] < other.data[i] ) {
                return true;
            } else if ( data[i] > other.data[i] ) {
                return false;
            }
        }
        // 루프를 다 돌았는데도 모든 공통위치 항목들이 같다면 결국 길이 차이로 판정. 
        // 공통부분이 같다면 짧은 벡터가 사전적으로 더 작다. 
        return length < other.length;
    }

    bool operator>(const MyVector& other) const { return other < *this; }

    bool operator<=(const MyVector& other) const { return !(*this > other); }

    bool operator>=(const MyVector& other) const { return !(*this < other); }


    // Vector에 대한 반복자
    class Iterator {
        T* ptr;         // 항목에 대한 포인터
    public:
        Iterator(T* p = nullptr): ptr(p) {}

        T& operator*() {   // 역참조 연산자 구현
            return *ptr;   // *it = x; 같은 대입 허용   
            //만약 T를 값으로 반환하면 복사가 발생. *it = 5; 같은 대입 불가
            // 수정 가능한 접근은 T&         
        }
        Iterator& operator++() { ++ptr; return *this;} // 전위 ++ 연산자, 반복자 자체를 이동시키는 것 
                                                        // 호출 후에 반복자(자기 자신)을 반환해야한다.
                                                        // 호출 후에 *this를 반환 ( 복사를 피해서 효율적 , 원래 객체를 수정하고 그 객체의 참조를 돌려줌 )
        
        Iterator& operator--() { --ptr; return *this; }   //전위 감소 연산자 구현
        Iterator operator+(int n) const { return Iterator(ptr + n); }   // 산술 연산자 구현
        Iterator operator-(int n) const { return Iterator(ptr - n);}  // 산술 연산자 구현

        bool operator==(const Iterator& other) const { return ptr == other.ptr;}
        bool operator!=(const Iterator& other) const { return ptr != other.ptr;}

        int operator-(const Iterator& other) const { return ptr - other.ptr;}
    };

    Iterator begin() { return Iterator(data); }  // 벡터의 첫 번쨰 원소를 가리키는 Iterator 생성 
    Iterator end() { return Iterator(data + length);}    
    // 벡터의 마지막 원소 다음을 가리키는 Iterator 생성, 
    // 이 위치는 역참조 x 반복범위 판단하는 기준임, 
    // 이렇게 해야 begin() == end()가 된다. 

    //벡터가 재할당(resize, push_back로 capacity 확장 등)되면 
    // 내부 data 포인터가 바뀌므로 기존의 모든 반복자는 무효화됩니다.
    //범위 기반 for는 begin()/end()가 사용 가능하고 반복자가 operator!=와 operator++, operator*를 제공해야 작동.


    // insert: 지정 위치에 요소 삽입 함수 구현
    Iterator insert(Iterator pos, const T& value) {  // pos가 가리키는 요소에 val 삽입
        // pos == begin() 이면 맨 앞에 삽입, pos == end() 이면 맨 뒤에 삽입
        // const 참조로 받아 복사를 최소화 
        size_t index = pos - begin(); // 삽입 위치의 인덱스 게산, begin() + 2 면 index == 2

        if ( length >= cap ) { // resize 연산, 저장된 요소 수(length)가 용량(cap)과 같거나 크면
            size_t new_cap = (cap == 0)? 1: cap * 2 ;
            T* new_data = new T[new_cap]; // 새로운 배열 동적 할당 
            
            for ( size_t i = 0; i < index; i++ ) { // 데이터 복사
                new_data[i] = data[i];
            }

            new_data[index] = value; // 새 요소 삽입
            for ( size_t i = index; i < length; i++ ) {
                new_data[i + 1] = data[i];
            }

            delete[] data;
            data = new_data;
            cap = new_cap;
            length++;

            return Iterator(data+ index); // 삽입된 위치의 Iterator(반복자) 반환
        
        } else { // resize x 
            for ( size_t i = length; i > index; i--)  {
                data[i] = data[i - 1];  // 끝 인덱스는 i - 1 , 한 칸 씩 뒤로 이동 
            }
            data[index] = value;
            ++length;
            return Iterator(data + index); // 삽입된 위치의 Iterator(반복자) 반환
        }
    }


    // erase: 지정 위치 요소 제거 함수 구현
    Iterator erase(Iterator pos) {
        size_t index = pos - begin(); // 삭제 위치의 인덱스 반환 

        for ( size_t i = index; i < length - 1; i++ ) {
            data[i] = data[ i + 1 ]; // 한 칸씩 앞으로 이동
        }

        --length;
        return Iterator(data + index); // 삭제된 위치의 다음 위치 Iterator 반환
    }

    // clear: 모든 요소 제거 함수 구현 (size = 0 초기화)
    void clear() { // size를 0으로 초기화
        length = 0;
    }

    // at: 범위 체크 후, i번째 항목 접근 함수 구현
    T& at(size_t i) { // 범위 체크 후 i 번째 항목 접근 함수
        for ( size_t idx = 0; idx < length; ++idx ) {
            if ( idx == i ) {
                return data[idx];
            }
        }

        throw std::out_of_range("index out of range");
    } 

    T& front() { // 첫번쨰 항목 리턴 함수
        if (length > 0) {
            return data[0];
        }
        throw std::out_of_range("vector is empty");
    }
    

    T& back() { // 마지막 항목 리턴 함수
        if ( length > 0 ) {
            return data[length - 1];
        }
        throw std::out_of_range("vector is empty");
    }

    // capacity
    size_t capacity() const { // cap값 리턴하는 함수 
        return cap;
    }
    
    // empty
    bool empty() const {
        return length == 0; // 벡터에 요소가 없으면 true
    }   // 현재 vector가 empty인지 확인하는 함수 구현

};