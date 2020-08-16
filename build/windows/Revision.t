#ifndef VERSION_H_ 
#define VERSION_H_ 

//major.minor.svn(git)version.datetime_base  
#define MAJOR_VERSION   ("0") 
#define MINOR_VERSION	("1") 
#if _WIN32 
#define SG_BRANCH_NAME	("$SG_BRANCH_NAME$") 
#define SG_COMMIT_HASH	("$SG_COMMIT_HASH$") 
#define SG_COMMIT_DATE	("$SG_COMMIT_DATE$") 
#define SG_VERSION		(SG_COMMIT_HASH) 
#else 
#ifdef ANDROID
#define SG_VERSION		(1000) //TODO: support git version for android platform
#else
#define SG_VERSION		(GIT_VERSION) 
#endif 
#endif /* #if _WIN32 */ 
#endif /* #ifndef VERSION_H_ */
