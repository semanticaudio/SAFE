#ifndef __SAFEWARNINGS__
#define __SAFEWARNINGS__

/** An enum type for the warning messages. */
enum WarningID
{
    NoWarning, /**< Nothing bad has happened. */
    LoadingDisabled, /**< No loading of descriptors allowed at this time. */
    AudioNotPlaying, /**< Audio needs to be playing to record it. */
    ParameterChange, /**< No parameter changes while audio is recording please. */
    AudioNotProcessed, /**< No timbral transformation has been applied to the signal. */
    AnalysisThreadBusy, /**< Busy analysing the last recorded audio. */
    DescriptorNotOnServer, /**< Can't load something that doesn't exist. */
    DescriptorNotInFile, /**< Can't load something that doesn't exist. */
    DescriptorBoxEmpty, /**< Can't load nothing. */
    CannotReachServer /**< No connection to the interwebz. */
};

#endif // __SAFEWARNINGS__
