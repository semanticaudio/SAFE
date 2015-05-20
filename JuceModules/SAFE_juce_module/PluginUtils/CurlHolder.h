#ifndef __CURLHOLDER__
#define __CURLHOLDER__

/**
 *  A class to manage the initialisation and cleanup of the cURL library.
 */
class CurlHolder
{
public:
    /** Set up an instance of cURL. */
    CurlHolder ()
    {
        curl = curl_easy_init();
    }

    /** Clean up and instance of cURL. */
    ~CurlHolder ()
    {
        curl_easy_cleanup (curl);
    }

CURL *curl; /**< The CURL object to use when calling cURL functions. */
};

#endif // __CURLHOLDER__
