[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/BOcBcCz-)
# MyVector 실습 (C++ 컨테이너 & Iterator 패턴)

## 개요
이번 실습의 목표는 **STL Vector의 기본 동작 원리**와 **Iterator 패턴**을 이해하고, 직접 구현해 보는 것입니다.
이번 실습에서는 `MyVector` 클래스를 작성하여 동적 배열 기능과 반복자(iterator)를 구현합니다.

## 실습 목적
- 사용자 정의 컨테이너 구현 
- **Iterator 패턴**을 적용하여 `MyVector`에 대한 iterator 구현

## MyVector 구현 내용 

### 1. 요소 추가 및 제거
- `push_back(value)`: 벡터 끝에 요소 추가
- `pop_back()`: 마지막 요소 제거
- `insert(pos, value)`: 지정 위치에 요소 삽입
- `erase(pos)`: 지정 위치 요소 제거
- `clear()`: 모든 요소 제거

### 2. 접근
- `operator[]`: 인덱스로 요소 접근 (범위 체크 없음)
- `at(index)`: 인덱스로 요소 접근 (범위 체크 있음)
- `front() / back()`: 첫 번째 / 마지막 요소 반환

### 3. 크기 관련
- `size()`: 현재 요소 개수
- `empty()`: 벡터가 비어 있는지 확인

### 4. 반복자 
- `begin() / end()`: 순방향 반복자
- Iterator는 `++`, `--`, `+`, `-`, 비교 연산자 지원
- Iterator는 `*` Dereference 연산자 지원

## 예시 코드
```cpp
#include "test.h"
#include <iostream>

int main() {
    MyVector<int> v = {10, 20, 30};
    v.push_back(40);
    v.insert(v.begin() + 1, 15);
    v.erase(v.begin() + 2);

    std::cout << "Elements: ";
    for (auto& x : v) std::cout << x << " "; // 10 15 30 40
    std::cout << "
Size: " << v.size() << "
"; // Size: 4

    std::cout << "Reverse: ";
    for (auto it = v.rbegin(); it != v.rend(); ++it)
        std::cout << *it << " "; // 40 30 15 10
}
```

## 예상 출력
```
[TEST1]
v.size(): 3
v[0]: 0
v[1]: 1
v[2]: 2
*it: 0
*it after ++: 1
Range-based for: 0 1 2

[TEST2]
a == b: 1
a != b: 0
a != c: 1
a < c: 1
c > a: 1
a <= b: 1
c >= a: 1
d < a: 1
a > d: 1
empty < a: 1
a > empty: 1
empty == empty: 1
a == a: 1
a < a: 0
a > a: 0

[TEST3]
Initial: 10 20 30
After push_back: 10 20 30 40
After pop_back: 10 20 30
After insert at pos 1: 10 15 20 30
After erase pos 2: 10 15 30
at(2): 15
front: 10
back: 30
After clear, size: 0
Empty: 1
```

## 테스트 방법

모든 함수 구현 후, 아래 명령어를 통해 테스트를 실행하세요:

### Windows 사용자
```
.\test10.bat
```

### MacOS/Linux 사용자
```bash
/bin/bash test10.sh
```

## 생각해보기
- `erase()`와 `insert()`에서 성능을 개선하려면 어떤 자료구조가 적합할까?
