#include "exact.h"
#include <iostream>
using namespace std;
int failures = 0;
void check(string name, bool ok) {
    cout << name << ": " << (ok ? "PASS" : "FAIL") << '\n';
    if (!ok) failures++;
}
int main() {
    HashTable t(1); // Force all keys into the same bucket.
    Entry out[100]; int n;
    for (int i=1; i<=4; i++)
        for (int j=0; j<10-2*i; j++) t.hashUpdate("10.0.0."+to_string(i));
    t.topK(3,out,n);
    check("TC01 exact counts and ranking", n==3 && out[0].key=="10.0.0.1" && out[0].count==8 && out[1].key=="10.0.0.2" && out[1].count==6 && out[2].key=="10.0.0.3" && out[2].count==4 && t.hashCount("10.0.0.4")==2);
    cout << "Sample run 1: top 3 of 20 records\n";
    for(int i=0;i<n;i++) cout << out[i].key << " " << out[i].count << '\n';
    check("Collision and missing key", t.uniqueCount()==4 && t.hashCount("missing")==0);
    HashTable empty; empty.topK(3,out,n); check("TC07 empty",n==0);
    empty.hashUpdate("10.0.0.7"); empty.topK(10,out,n); check("TC09 one record",n==1 && out[0].count==1 && out[0].key=="10.0.0.7");
    HashTable five;
    for(int i=1;i<=5;i++) for(int j=0;j<i;j++) five.hashUpdate("10.0.0."+to_string(20+i));
    five.topK(100,out,n); bool ok=n==5; for(int i=0;i<n;i++) ok=ok && out[i].count==5-i && out[i].key=="10.0.0."+to_string(25-i);
    check("TC10 oversized k",ok);
    cout << "Sample run 2: k=100, 15 records\n"; for(int i=0;i<n;i++) cout<<out[i].key<<" "<<out[i].count<<'\n';
    five.topK(1,out,n); check("TC11 k=1",n==1 && out[0].key=="10.0.0.25" && out[0].count==5);
    five.topK(-5,out,n); check("TC18 negative k (component)",n==0);
    HashTable ties(1); ties.hashUpdate("A"); ties.hashUpdate("B"); ties.topK(1,out,n); check("Tie ordering",n==1 && out[0].key=="A");
    HashTable large; for(int i=0;i<10000;i++) large.hashUpdate("10.0.0."+to_string(i%100+1));
    large.topK(100,out,n); ok=n==100; for(int i=1;i<=100;i++) ok=ok && large.hashCount("10.0.0."+to_string(i))==100;
    check("10000 records",ok);
    cout << "Failures: " << failures << '\n'; return failures!=0;
}
