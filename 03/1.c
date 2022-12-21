STYPE
bit_reverse(STYPE value)
{
    UTYPE res = 0;
    UTYPE opposite = value | ~value;
    while (opposite > 0) {
        res <<= 1;
        res += value & 1;
        opposite >>= 1;
        value >>= 1;
    }
    return (STYPE) res;
}
