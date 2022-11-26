enum { OFFSET_BITS = 9 };
enum { PAGE_BITS = 23 };
enum { NO_PAGE = 0xffffffff};

unsigned To_PA(const unsigned *Page_Tab, unsigned  Virt_A, int PID) {
    unsigned page = Virt_A >> OFFSET_BITS;
    if (Page_Tab[page] == NO_PAGE) {
        kill(PID, SIGPL);
        exit(No_Page);
    }
    unsigned addr = Page_Tab[page];
    return ((Virt_A << PAGE_BITS) >> PAGE_BITS) + addr;
}