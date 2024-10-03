#include <iostream>
#include <format>

void showValue(const std::string& msg, unsigned long long v)
{
    std::cout << std::format("{:15}: v={:x} double={}", msg, v, *(double*)&v) << std::endl;
}

/*
no_mans_land (0xFD)
The "no_mans_land" buffers on either side of the memory used by an application are currently filled with 0xFD.

Freed blocks (0xDD)
The freed blocks kept unused in the debug heap's linked list when the _CRTDBG_DELAY_FREE_MEM_DF flag is set are currently filled with 0xDD.

New objects (0xCD)


https://learn.microsoft.com/en-us/cpp/c-runtime-library/crt-debug-heap-details?view=msvc-170

Value     Name           Description
------   --------        -------------------------
0xCD     Clean Memory    Allocated memory via malloc or new but never
                         written by the application.

0xDD     Dead Memory     Memory that has been released with delete or free.
                         It is used to detect writing through dangling pointers.

0xED or  Aligned Fence   'No man's land' for aligned allocations. Using a
0xBD                     different value here than 0xFD allows the runtime
                         to detect not only writing outside the allocation,
                         but to also identify mixing alignment-specific
                         allocation/deallocation routines with the regular
                         ones.

0xFD     Fence Memory    Also known as "no mans land." This is used to wrap
                         the allocated memory (surrounding it with a fence)
                         and is used to detect indexing arrays out of
                         bounds or other accesses (especially writes) past
                         the end (or start) of an allocated block.

0xFD or  Buffer slack    Used to fill slack space in some memory buffers
0xFE                     (unused parts of `std::string` or the user buffer
                         passed to `fread()`). 0xFD is used in VS 2005 (maybe
                         some prior versions, too), 0xFE is used in VS 2008
                         and later.

0xCC                     When the code is compiled with the /GZ option,
                         uninitialized variables are automatically assigned
                         to this value (at byte level).


// the following magic values are done by the OS, not the C runtime:

0xAB  (Allocated Block?) Memory allocated by LocalAlloc().

0xBAADF00D Bad Food      Memory allocated by LocalAlloc() with LMEM_FIXED,but
                         not yet written to.

0xFEEEFEEE               OS fill heap memory, which was marked for usage,
                         but wasn't allocated by HeapAlloc() or LocalAlloc().
                         Or that memory just has been freed by HeapFree().

https://stackoverflow.com/questions/370195/when-and-why-will-a-compiler-initialise-memory-to-0xcd-0xdd-etc-on-malloc-fre

*/


int main(int argc, char** argv)
{
    std::cout << "Hello world" << std::endl;

    showValue("Just allocated",              0xcdcdcdcdcdcdcdcd);
    showValue("Freed memory",                0xdddddddddddddddd);

    showValue("Aligned Fence",               0xedededededededed);
    showValue("Aligned Fence",               0xbdbdbdbdbdbdbdbd);
    showValue("Fence Memory",                0xfdfdfdfdfdfdfdfd);

    showValue("Buffer slack",                0xfefefefefefefefe);

    showValue("Uninitialized",               0xcccccccccccccccc);

    showValue("Local alloc",                 0xabababababababab);
    showValue("Bad Food",                    0xBAADF00DBAADF00D);
    showValue("0xFEEEFEEE",                  0xFEEEFEEEFEEEFEEE);

 //   return 0;


    double* p = new double[100];
    double* w = p;
    unsigned long long* z = (unsigned long long* )p;
    delete p;
    
    z[1] = 0xcccccccccccccccc;

    std::cout << std::format("{}", w[1]) << std::endl;
    std::cout << std::format("{:x}", z[1]) << std::endl;

}