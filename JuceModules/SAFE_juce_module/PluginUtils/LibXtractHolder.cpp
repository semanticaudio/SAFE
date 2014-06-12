LibXtractHolder::LibXtractHolder ()
{
    // initialise the libxtract fft stuff
    xtract_init_fft (ANALYSIS_FRAME_LENGTH, XTRACT_SPECTRUM);

    barkBandLimits.allocate (26, true);

    melFilters.n_filters = 13;
    melFilters.filters = new double* [13];

    for (int n = 0; n < 13; ++n)
    {
        melFilters.filters [n] = new double [ANALYSIS_FRAME_LENGTH];
    }
}

LibXtractHolder::~LibXtractHolder ()
{
    for (int n = 0; n < 13; ++n)
    {
        delete[] melFilters.filters [n];
    }

    delete[] melFilters.filters;

    // free the libxtract fft stuff (important when we link statically)
    xtract_free_fft();
}
