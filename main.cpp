#include <bits/stdc++.h>
#define int64 uint64_t
using namespace std;
using namespace chrono;
typedef long long ll;

const int p = ((uint64_t)1)<<31-1;
const int q=31;
int r=1<<20;

vector<list<pair<uint64_t,uint64_t>>> chaining(1);

class hash_family{
public:
    void update(int i, int value);
    uint64_t Query() {
        uint64_t total = 0;
        for (int i = 0; i < chaining.size(); i++) {
            list<pair<uint64_t, uint64_t>>::iterator it;
            for (it = chaining[i].begin(); it != chaining[i].end(); it++) {
                total += it->second * it->second;
            }
        }
        return total;
    }
private:

};



class hash_with_chain:public hash_family{
public:
    uint64_t a;
    hash_with_chain() {
        do {
            a = rand() % p;
        } while (a % 2 == 0);
    };
    uint64_t hash_f1(uint64_t x) {
        return ((a*x)) >> (64-31);
    }
    void update(uint64_t i, uint64_t value, uint64_t a){
        int k= hash_f1(i);
        list<pair<uint64_t,uint64_t>>::iterator it;
        for(it = chaining[k].begin();it!=chaining[k].end();it++){
            if(it->first==i) {
                it->second += value;
                return;
            }
        }
        chaining[k].push_back(make_pair(i,value));
    }

};

class four_wise_hash:public hash_family{
public:
    uint64_t a,b,c,d,r;
    uint64_t mod(uint64_t a, uint64_t x){
        uint64_t y1=(x&p)+(x>>q);
        if (y1>=p) y1-=p;
        uint64_t y2=(a&p)+(a>>q);
        if (y2>=p) y2-=p;
        uint64_t y3=y1*y2;
        y3=(y3&p)+(y3>>q);
        if (y3>=p) y3-=p;
        return y3;
    }

    uint64_t hash_f2(uint64_t x) {
        uint64_t k=(mod(a,x)+b)%p;
        k=(mod(k,x)+c)%p;
        k=(mod(k,x)+d)%p;
        return k;
    }

    uint64_t h(uint64_t k) {
        return (k >> 1) & (r-1);
    }

    uint64_t g(uint64_t k) {
        return 2*(k & 1)-1;;
    }


    void update(uint64_t i, uint64_t value){
        int k= hash_f2(i);
        list<pair<uint64_t,uint64_t>>::iterator it;
        for(it = chaining[k].begin();it!=chaining[k].end();it++){
            if(it->first==i) {
                it->second += value;
                return;
            }
        }
        chaining[k].push_back(make_pair(i,value));
    }


    four_wise_hash(uint64_t r=1<<20) {
        a = rand()%p;
        b = rand()%p;
        c = rand()%p;
        d = rand()%p;
        r=r;
    }
};

set<int64> rb;




int64 capc(int64 x) {
    return (int64)(4.0*log((double)x)/log(log((double)x)));
}









int main() {
    cout<<1<<endl;
    int maxn=1e6;
    auto start = system_clock::now();
    four_wise_hash *ff=new four_wise_hash();
    for(int i=0;i<maxn;i++){
        uint64_t k=ff->hash_f2(i);
        ff->h(k);
        ff->g(k);
    }


    auto end = system_clock::now();

    auto duration1 = duration_cast<microseconds>(end - start);

    cout<<double(duration1.count())* microseconds::period::num / microseconds::period::den<<endl;

    return 0;
}


