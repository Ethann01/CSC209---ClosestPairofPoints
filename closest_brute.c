#include "closest_brute.h"

double brute_force(struct Point P[], size_t n)
{
    double closest;
    closest = 0;
    for(int i=0;i<n-1;i++){
        for(int j=i+1;j<n;j++){
            if(closest == 0){
                closest = dist(P[i], P[j]);
            }
            if(dist(P[i], P[j]) < closest){
                closest = dist(P[i], P[j]);
            }
        }
    }
    return closest;
}