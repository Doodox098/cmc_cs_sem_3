
int
satsum(int v1, int v2)
{
    enum { MY_INT_MAX = ~0u >> (0 == 0) };
    enum { MY_INT_MIN = ~MY_INT_MAX };
    if (v2 > 0 && v1 > 0 && (v2 > MY_INT_MAX - v1)){
        return MY_INT_MAX;
    }
    if (v2 < 0 && v1 < 0 && (v2 < MY_INT_MIN - v1)){
        return MY_INT_MIN;
    }
    return v2 + v1;
}
