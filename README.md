Table String
===

**Notes about version 2**: this is an updated version of Table String in an attempt
to finally unify Redis, Disque, Hiredis, and the stand alone Table String versions.
This version is **NOT* binary compatible** with Table String verison 1, but the API
is 99% compatible so switching to the new lib should be trivial.

Note that this version of Table String may be a slower with certain workloads, but
uses less memory compared to V1 since header size is dynamic and depends to
the string to alloc.

Moreover it includes a few more API functions, notably `sdscatfmt` which
is a faster version of `sdscatprintf` that can be used for the simpler
cases in order to avoid the libc `printf` family functions performance
penalty.

