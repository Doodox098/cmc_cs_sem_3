STYPE bit_reverse(STYPE value) {
    UTYPE value_new = 0;
    UTYPE value_extra_1 = ~value;
    UTYPE value_extra_2 = value;
    while (value_extra_1 != 0 || value_extra_2 != 0) {
        value_new = value_new << 1;
        value_new += value_extra_2 & 1;
        value_extra_1 = value_extra_1 >> 1;
        value_extra_2 = value_extra_2 >> 1;
    }
    return (STYPE)value_new;
}