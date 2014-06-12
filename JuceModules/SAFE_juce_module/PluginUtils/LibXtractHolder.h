#ifndef __LIBXTRACTHOLDER__
#define __LIBXTRACTHOLDER__

class LibXtractHolder
{
public:
    LibXtractHolder ();
    ~LibXtractHolder ();

    HeapBlock <int> barkBandLimits;

    xtract_mel_filter melFilters;

};

#endif // __LIBXTRACTHOLDER__
