int
satsum(int v1, int v2)
{
    enum { MY_INT_MAX = (1U << 31) - 1 };
    enum { MY_INT_MIN = 1U << 31 };
    int Middle;
    if (v1 > 0 && v2 > 0) {
        Middle = MY_INT_MAX >> 1;
        if (v1 > Middle && v2 > Middle) return MY_INT_MAX;
        if (v1 > Middle && v2 + (v1 - Middle) > Middle + 1) return MY_INT_MAX;
        if (v2 > Middle && v1 + (v2 - Middle) > Middle + 1) return MY_INT_MAX;
    }
    if (v1 < 0 && v2 < 0) {
        Middle = -(MY_INT_MIN >> 1);
        if ((v1 < Middle && v2 < Middle) || (v1 < Middle && v2 == Middle) || (v2 < Middle && v1 == Middle))
            return MY_INT_MIN;
        if (v1 < Middle && v2 + (v1 - Middle) < Middle) return MY_INT_MIN;
        if (v2 < Middle && v1 + (v2 - Middle) < Middle) return MY_INT_MIN;

    }
    return v1 + v2;
}
