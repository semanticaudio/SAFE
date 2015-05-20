#ifndef __LIBXTRACTHOLDER__
#define __LIBXTRACTHOLDER__

/**
 *  A class to manage the initialisation and cleanup of the libXtract library.
 */
class LibXtractHolder
{
public:
    /** Set up an instance of libXtract. */
    LibXtractHolder ();

    /** Clean up and instance of libXtract. */
    ~LibXtractHolder ();

    HeapBlock <int> barkBandLimits; /**< The libXtract bark band limits. */

    xtract_mel_filter melFilters; /**< The libXtract mel filters. */

};

#endif // __LIBXTRACTHOLDER__
