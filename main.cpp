#include <bits/stdc++.h>
#define int64 uint64_t
using namespace std;
using namespace chrono;
typedef long long ll;

const int p = ((uint64_t)1)<<31-1;
const int q=31;

class hash_family{
public:
    virtual void update(uint64_t i, uint64_t value)=0;

};



class hash_with_chain:public hash_family{
public:
    uint64_t a;
    vector<unordered_map<uint64_t,uint64_t>> chaining;
    hash_with_chain() {
        do {
            a = rand() % p;
        } while (a % 2 == 0);
        chaining.resize(p);
    };
    uint64_t hash_f1(uint64_t x) {
        return ((a*x)) >> (64-31);
    }
    void update(uint64_t i, uint64_t value){
        int k= hash_f1(i);
        unordered_map<uint64_t,uint64_t>::iterator it;
        for(it = chaining[k].begin();it!=chaining[k].end();it++){
            if(it->first==i) {
                it->second += value;
                return;
            }
        }
        chaining[k][i]=value;
    }

    uint64_t Query() {
      uint64_t total = 0;
        for (int i = 0; i < chaining.size(); i++) {
            unordered_map<uint64_t, uint64_t>::iterator it;
            for (it = chaining[i].begin(); it != chaining[i].end(); it++) {
                total += it->second * it->second;
            }
        }
        return total;
    }
  ~hash_with_chain() {

  }
};

class four_wise_hash:public hash_family{
public:
    uint64_t a,b,c,d,r;
    vector<uint64_t> values;
    uint64_t mod(uint64_t a) {
      uint64_t y = (a&p) + (a>>q);
      if(y>=p) y-=p;
      return y;
    }
    uint64_t hash_f2(uint64_t x) {
      uint64_t k=mod(mod(a*mod(x))+b);
      k = mod(mod(k*mod(x))+c);
      k=mod(mod(k*mod(x))+d);
      return k;
    }

    uint64_t h(uint64_t k) {
        return (k >> 1) & (r-1);
    }

    int g(uint64_t k) {
      uint64_t kk = k & (uint64_t)1;
      return 2*(int)(kk)-1;;
    }


    void update(uint64_t i, uint64_t value){
        int k= hash_f2(i);
	if(g(k)==-1)
	  values[h(k)] -= value;
	else values[h(k)] += value;
    }

    uint64_t Query() {
        uint64_t total = 0;
        for (int i = 0; i < values.size(); i++) {
           total+=values[i]*values[i];
        }
        return total;
    }


    four_wise_hash(uint64_t r=1<<20) {
        a = rand()%p;
        b = rand()%p;
        c = rand()%p;
        d = rand()%p;
        r=r;
        values.resize(r);
    }

  ~four_wise_hash() {

  }
};

class test{
public:
  void ex5() {

  }
  void ex7() {
    for(int i = (1<<6); i <= (1<<28); i=i*2) {
      vector<hash_family*> v;
      cerr << 1 << endl;
      v.push_back(new hash_with_chain());
      cerr << 2 << endl;
      v.push_back(new four_wise_hash((uint64_t)(1<<7)));
      cerr << 3 << endl;
      v.push_back(new four_wise_hash((uint64_t)(1<<10)));
      cerr << 4 << endl;
      v.push_back(new four_wise_hash((uint64_t)(1<<20)));
      for(int j = 0; j < v.size(); ++j) {
	hash_family* ptr = v[j];
	auto start = system_clock::now();
	for(int k=1; k<=1000000; ++k) {
	  if(k%10000000==0)
	    cerr << "i,j,k:" << i <<" " << j<< " " << k << endl;
	  ptr->update(k%i,1);
	}
	auto end = system_clock::now();
	auto dur = duration_cast<microseconds>(end-start);
	cout << double(dur.count()) * microseconds::period::num / microseconds::period::den << " ";	
      }
      cout << endl;
      for(int j = 0; j < v.size(); ++j) {
	delete v[j];
      }
    }
  }
};

int main() {
  test* ptr = new test();
  ptr->ex7();
  return 0;
}


