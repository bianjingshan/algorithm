
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <map>
#include <algorithm>
#include <sys/time.h>
using namespace std;


class ClassA {
 public:
    ClassA() {
        
    }
    ~ClassA() {

    }
    void test() {
        printf("test\n");
    }
};

double CurrentTimeMsec(void) {
    timeval m_tv;
    gettimeofday(&m_tv, NULL);
    return (double)m_tv.tv_sec * 1000.0 + (double)m_tv.tv_usec / 1000.0;
}

void VectorTest(void) {
    ClassA *find_value = (ClassA *)123;
    vector<ClassA *> v_a;
    for (int i = 0; i < 256; i++) {
        v_a.push_back(new ClassA());
    }
    for (int i = 0; i< 1000000; i++) {
        if (find(v_a.begin(), v_a.end(), find_value) != v_a.end()) {
            printf("find item\n");
        }
    }
}

void MapTest(void) {
    ClassA *find_value = (ClassA *)123;
    std::map<ClassA *, bool> m_a;
    for (int iloop = 0; iloop < 256; iloop++) {
        m_a[new ClassA()] = true;
    }
    for (int i = 0; i< 1000000; i++) {
        if (m_a.count(find_value) > 0) {
            printf("find item\n");
        }
    }
}

int main(int argc, char *argv[]) {
    double start_time, end_time;
    start_time = CurrentTimeMsec();
    VectorTest();
    end_time = CurrentTimeMsec();
    printf("VectorTest:%.6f ms\n", end_time - start_time);

    start_time = CurrentTimeMsec();
    MapTest();
    end_time = CurrentTimeMsec();
    printf("MapTest:%.6f ms\n", end_time - start_time);
    return 0;
}