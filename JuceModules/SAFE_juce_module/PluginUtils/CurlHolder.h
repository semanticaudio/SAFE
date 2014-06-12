#ifndef __CURLHOLDER__
#define __CURLHOLDER__

class CurlHolder
{
public:
    CurlHolder ()
    {
        curl = curl_easy_init();
    }

    ~CurlHolder ()
    {
        curl_easy_cleanup (curl);
    }

CURL *curl;
};

#endif // __CURLHOLDER__
