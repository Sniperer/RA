#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
using namespace chrono;
typedef long long ll;

const unsigned int p = (((uint64_t)1) << 31) - 1;
const int q = 31;

class hash_family {
public:
    virtual void update(uint64_t i, uint64_t value) = 0;
    hash_family() {}
    ~hash_family() {}
};

pair<uint64_t, uint64_t>* chaining;

class hash_with_chain :public hash_family {
public:
    uint64_t a;
    hash_with_chain() {
        a = 0xc6a4a7935bd1e995;
    };
    uint64_t hash_f1(uint64_t x) {
        return ((a * x)) >> (64 - 31);
    }
    void update(uint64_t i, uint64_t value) {
        uint32_t k = hash_f1(i);
        bool flag = 0;
        for (uint32_t j = 0; j < 2; ++j) {
            if (chaining[k*2 + j].first == i) {
                chaining[k*2 + j].second += value;
                flag = 1;
            }
        }
        if (flag)
            return;
        chaining[k*2+1] = {i,value};
    }

    uint64_t Query() {
        uint64_t total = 0;
        for (uint32_t i = 0; i < p; i++) {
            for (uint32_t j = 0; j < 2; ++j) {
                uint64_t tmp = chaining[i * 2 + j].second;
                total += tmp*tmp;
            }
        }
        return total;
    }
    ~hash_with_chain() {
        memset(chaining, 0, sizeof(pair<uint64_t, uint64_t>) * p * 2);
    }
};

vector<uint64_t> values;

class four_wise_hash :public hash_family {
public:
    
    four_wise_hash(uint64_t aa): r(aa){
        a = rand() % p;
        b = rand() % p;
        c = rand() % p;
        d = rand() % p;
        values.resize(r);
    }

    ~four_wise_hash() {
        values.clear();
    }
    uint64_t a, b, c, d, r;

    uint64_t mod(uint64_t a) {
        uint64_t y = (a & p) + (a >> q);
        if (y >= p) y -= p;
        return y;
    }
    uint64_t hash_f2(uint64_t x) {
        uint64_t k = mod(mod(a * mod(x)) + b);
        k = mod(mod(k * mod(x)) + c);
        k = mod(mod(k * mod(x)) + d);
        return k;
    }

    uint64_t h(uint64_t k) {
        return (k >> 1) & (r - 1);
    }

    int g(uint64_t k) {
        uint64_t kk = k & (uint64_t)1;
        return 2 * (int)(kk)-1;;
    }


    void update(uint64_t i, uint64_t value) {
        int k = hash_f2(i);
        if (g(k) == -1)
            values[h(k)] -= value;
        else values[h(k)] += value;
    }

    uint64_t Query() {
        uint64_t total = 0;
        for (int i = 0; i < values.size(); i++) {
            total += values[i] * values[i];
        }
        return total;
    }



};

class test {
public:
    void ex5() {

    }
    void ex7() {
        for (int i = (1 <<27); i <= (1 << 28); i = i * 2) {
            cout << i << endl;
            hash_with_chain v0;
            four_wise_hash v1(1 << 7);
            four_wise_hash v2(1 << 10);
            four_wise_hash v3(1 << 20);
            for (int j = 0; j < 4; ++j) {
                hash_family* ptr;
                if (j == 0) ptr = &v0;
                else if (j == 1) ptr = &v1;
                else if (j == 2) ptr = &v2;
                else ptr = &v3;
                auto start = system_clock::now();
                for (int k = 1; k <= 10000000; ++k) {
                    //if (k % 10000 == 0)
                    //    cout << "i,j,k:" << i << " " << j << " " << k << endl;
                    ptr->update(k % i, 1);
                }
                auto end = system_clock::now();
                auto dur = duration_cast<microseconds>(end - start);
                cout << double(dur.count()) * microseconds::period::num / microseconds::period::den << ",";
            }
            cout << endl;
        }
    }
};

int main() {
    chaining = (pair<uint64_t, uint64_t>*)malloc(sizeof(pair<uint64_t, uint64_t>) * p * 2);
//    for (int i = 0; i < p; ++i) {
//        if(i%10000000==0)
//            cout << i << endl;
//        chaining[i] = (pair<uint64_t, uint64_t>*)malloc(sizeof(pair<uint64_t, uint64_t>) * 3);
//    }
    test* ptr = new test();
    ptr->ex7();
    return 0;
}
